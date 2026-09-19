#ifndef OCMAP_H
#define OCMAP_H



#include "OTBackend/OTPath.h"
#include "OTBackend/OTPlatform.h"
#include <QDate>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QString>
#include <QStringConverter>
#include <QTextStream>
#include <QTime>
#include <Qt>
#include <algorithm>
#include "OCBase.h"

namespace OCMap {

class Position
{
public:
    OCType::Coord2D<int> tile;

    OCType::Coord3D<double> position;

    double rotAroundZ = 0.0;

    double rotAroundX = 0.0;

    double distanceFromPosition = 0.0;
};

class Map : public OCBase::File { // TODO: override clear function
public:
    class Global : public OCBase::File // global.cfg
    {
    public:
        class Texture
        {
        public:
            QString mainTex;

            QString subTex;

            int texSizeExponent = 0;

            int mainTexRepeating = 0;

            int subTexRepeating = 0;
        };

        class Season
        {
        public:
            enum Type
            {
                summer = 0,
                spring = 1,
                autumn = 2,
                winter = 3,
                deepWinter = 4
            };

            QString name()
            {
                switch (type)
                {
                case 1: return QObject::tr("Spring");
                case 2: return QObject::tr("Autumn");
                case 3: return QObject::tr("Winter");
                case 4: return QObject::tr("Deep winter");
                default: return QObject::tr("Summer");
                }
            }

            Type type;

            QDate start;

            QDate end;
        };

        QList<Season> sortSeasons(QList<Season> seasons)
        {
            QList<Season> list = seasons;

            std::sort(list.begin(), list.end(), [](const Season &a, const Season &b) {
                return a.start.dayOfYear() < b.end.dayOfYear();
            });
            return list;
        }

        class AiDensity
        {
        public:
            QTime time;

            double factor = 0.0;
        };

        // Wrap class for entrypoints
        class EntrypointCollection
        {
        public:
            class Entrypoint
            {
            public: // TODO
                /* [entrypoints]
             * entrypointCount      * (
             * objectID
             * ?
             * xpos
             * zpos            Attention! Inverted
             * ypos
             * ?        \
             * ?        |  maybe       | very awkward dependencies
             * ?        |  irrelevant  | with rot, pitch and bank
             * ?        /
             * tileID  // see order in global.cfg
             * name
             * )
             */

                int objectID = 0;

                int globalThingID = 0;

                int awkwardValue1 = 0;

                OCType::Coord3D<double> position;

                double awkwardValue2 = 0.0;

                double awkwardValue3 = 0.0;

                double awkwardValue4 = 0.0;

                double awkwardValue5 = 0.0;

                int tileID = 0;
            };
            QString name;

            QList<Entrypoint> entrypoints;
        };

        class BackgroundImage
        {
        public:
            bool isVisible = false;

            QString picturePath;

            double width = 0.0;

            double height = 0.0;

            double startWidth = 0.0;

            double startHeight = 0.0;
        };

        class TileInformation
        {
        public:
            OCType::Coord2D<int> position;

            QString filename;
        };

        QString name;

        QString friendlyname;

        QString description;

        int version; // mostly 14

        unsigned int nextIDCode = 0;

        bool worldCoodinates = false;

        bool dynHelpers = false;

        bool lht = false;

        bool realrail = false;

        BackgroundImage bgImage;

        Position standardView;

        QString currency;

        QString ticketpack;

        QTime repairTime;

        int startYear = 0;

        int endYear = 0;

        int realYearOffset = 0;

        QString standardDepot;

        QList<Texture> groundTextures;

        QList<Season> seasons;

        QList<AiDensity> trafficDensities;

        QList<AiDensity> passengerDensities;

        QList<EntrypointCollection> entrypoints;

        QList<TileInformation> tiles;

        /// Format global.cfg was read in, so write() can reproduce it byte for byte.
        OTPlatform::TextFormat fileFormat;

        FileIOResponse read() override
        {
            if (dir.isEmpty())
            {
                qWarning() << "Reading process of Global stopped: No file path given.";
                return FileIOResponse::errFileDoesntExist;
            }

            QFile global(OTPath::resolve(dir, "global.cfg"));

            if (!global.open(QFile::ReadOnly | QFile::Text))
            {
                // msg.fileOpenErrorCloseOMSI(parent, mapFolderPath); TODO
                qDebug().noquote() << "Cannot open file: Full path: '" + QFileInfo(global).absoluteFilePath() + "'";
                return FileIOResponse::errFileNotOpen;
            }

            // The OMSI editor writes UTF-16LE with a byte order mark, the shipped maps
            // are ANSI. Latin1 stays the fallback for files without a mark, which is
            // what this reader assumed for all of them before.
            fileFormat = OTPlatform::detectTextFormat(global, QStringConverter::Latin1);

            QTextStream in(&global);
            in.setEncoding(fileFormat.encoding);
            QString line = "";

            clear();

            try
            {
                while (!in.atEnd())
                {
                    line = in.readLine();

                    if (line == "[name]") name = in.readLine();
                    else if (line == "[friendlyname]") friendlyname = in.readLine();
                    else if (line == "[description]")
                    {
                        line = in.readLine();
                        description.clear();

                        while (line != "[end]")
                        {
                            description += line +  "\n";
                            line = in.readLine();
                        }

                        description = description.trimmed();
                    }
                    else if (line == "[version]") version = in.readLine().toInt();
                    else if (line == "[NextIDCode]") nextIDCode = in.readLine().toUInt();
                    else if (line == "[worldcoordinates]") worldCoodinates = true;
                    else if (line == "[dynhelperactive]") dynHelpers = true;
                    else if (line == "[LHT]") lht = true;
                    else if (line == "[realrail]") realrail = true;
                    else if (line == "[backgroundimage]")
                    {
                        bgImage.isVisible = in.readLine().toInt(); // 1
                        bgImage.picturePath = in.readLine(); // 2
                        bgImage.width = in.readLine().toDouble(); // 3
                        bgImage.height = in.readLine().toDouble(); // 4
                        bgImage.startWidth = in.readLine().toDouble(); // 5
                        bgImage.startHeight = in.readLine().toDouble(); // 6
                    }
                    else if (line == "[mapcam]")
                    {
                        // Attention: Inverted values
                        int yTilePos = in.readLine().toInt();
                        int xTilePos = in.readLine().toInt();

                        standardView.tile = OCType::Coord2D<int>(xTilePos, yTilePos); // 1, 2

                        // Attention: Inverted values
                        double xPos = in.readLine().toDouble(); // 3
                        double zPos = in.readLine().toDouble(); // 4
                        double yPos = in.readLine().toDouble(); // 5
                        standardView.position = OCType::Coord3D<double>(xPos, yPos, zPos); // 3, 4, 5

                        standardView.rotAroundZ = in.readLine().toDouble();
                        standardView.rotAroundX = in.readLine().toDouble();
                        standardView.distanceFromPosition = in.readLine().toDouble();
                    }
                    else if (line == "[moneysystem]") currency = in.readLine();
                    else if (line == "[ticketpack]") ticketpack = in.readLine();
                    else if (line == "[repair_time_min]")
                    {
                        /*
                            The value is in minutes - that is what the field is called, and
                            what shipped maps contain (1.000 / 5.000 / 10.000).

                            The remainder of the hour conversion used to be multiplied by 60
                            a second time, so 5 minutes turned into QTime(0, 300) - an
                            invalid QTime, whose hour() then returns -1 further down.
                        */
                        const int totalMinutes = qBound(0, qRound(in.readLine().toDouble()), 23 * 60 + 59);
                        repairTime = QTime(totalMinutes / 60, totalMinutes % 60);
                    }
                    else if (line == "[years]")
                    {
                        startYear = in.readLine().toInt();
                        endYear = in.readLine().toInt();
                    }
                    else if (line == "[standarddepot]") standardDepot = in.readLine();
                    else if (line == "[realyearoffset]") realYearOffset = in.readLine().toInt();
                    else if (line == "[groundtex]")
                    {
                        Texture groundtex;
                        groundtex.mainTex = in.readLine();
                        groundtex.subTex = in.readLine();
                        groundtex.texSizeExponent = in.readLine().toInt();
                        groundtex.mainTexRepeating = in.readLine().toInt();
                        groundtex.subTexRepeating = in.readLine().toInt();
                        groundTextures.append(groundtex);
                    }
                    else if (line == "[addseason]")
                    {
                        Season season;

                        int type = in.readLine().toInt();
                        switch (type)
                        {
                        case 1: season.type = Season::Type::spring; break;
                        case 2: season.type = Season::Type::autumn; break;
                        case 3: season.type = Season::Type::winter; break;
                        case 4: season.type = Season::Type::deepWinter; break;
                        default: season.type = Season::Type::summer; break;
                        };

                        season.start = QDate(QDate::currentDate().year(), 1, 1).addDays(in.readLine().toInt());
                        season.end = QDate(QDate::currentDate().year(), 1, 1).addDays(in.readLine().toInt());

                        seasons.append(season);
                    }
                    else if (line == "[trafficdensity_road]")
                    {
                        AiDensity density;
                        float decimalTime = in.readLine().toDouble(); int hours = static_cast<int>(decimalTime);
                        // TODO: Workaround
                        density.time = (hours != 24) ? QTime(hours, (decimalTime - hours) * 60) : QTime(23, 59);
                        density.factor = in.readLine().toDouble();
                        trafficDensities.append(density);
                    }
                    else if (line == "[trafficdensity_passenger]")
                    {
                        AiDensity density;
                        float decimalTime = in.readLine().toDouble(); int hours = static_cast<int>(decimalTime);
                        // TODO: Workaround
                        density.time = (hours != 24) ? QTime(hours, (decimalTime - hours) * 60) : QTime(23, 59);
                        density.factor = in.readLine().toDouble();
                        passengerDensities.append(density);
                    }
                    else if (line == "[entrypoints]")
                    {
                        int entrypointCount = in.readLine().toInt();

                        for (int i = 0; i < entrypointCount; i++)
                        {
                            EntrypointCollection::Entrypoint entrypoint;
                            entrypoint.objectID = in.readLine().toInt();
                            entrypoint.globalThingID = in.readLine().toInt();
                            entrypoint.awkwardValue1 = in.readLine().toInt();

                            // Attention: Inverted values
                            double xPos = in.readLine().toDouble();
                            double zPos = in.readLine().toDouble();
                            double yPos = in.readLine().toDouble();
                            entrypoint.position = OCType::Coord3D<double>(xPos, yPos, zPos);

                            entrypoint.awkwardValue2 = in.readLine().toDouble();
                            entrypoint.awkwardValue3 = in.readLine().toDouble();
                            entrypoint.awkwardValue4 = in.readLine().toDouble();
                            entrypoint.awkwardValue5 = in.readLine().toDouble();
                            entrypoint.tileID = in.readLine().toInt();

                            EntrypointCollection collection;
                            collection.name = in.readLine();
                            collection.entrypoints.append(entrypoint);

                            int collectionIndex = -1;
                            for (int j = 0; j < entrypoints.count(); j++) { if (entrypoints[j].name == collection.name) { collectionIndex = j; break; } }

                            if (collectionIndex != -1)
                                entrypoints[collectionIndex].entrypoints.append(entrypoint);
                            else entrypoints.append(collection);
                        }
                    }
                    else if (line == "[map]")
                    {
                        TileInformation tile;

                        int x = in.readLine().toInt();
                        int y = in.readLine().toInt();

                        tile.position = OCType::Coord2D<int>(x, y);
                        tile.filename = in.readLine();

                        tiles.append(tile);
                    }
                }

                global.close();
            }
            catch (...)
            {
                global.close();
                return FileIOResponse::errCritical;
            }

            seasons = sortSeasons(seasons);

            return FileIOResponse::valid;
        }

        FileIOResponse write() override
        {
            if (dir.isEmpty())
            {
                qWarning() << "Writing process of Global stopped: No file path given.";
                return FileIOResponse::errFileDoesntExist;
            }

            QFile global(OTPath::resolve(dir, "global.cfg"));

            // Backup
            if (!QDir().exists(dir + "/backup")) qDebug() << "Backup dir create:" << QDir().mkdir(dir + "/backup");
            if (QFile(dir + "/backup/global.cfg").exists()) QFile(dir + "/backup/global.cfg").remove();
            global.copy(dir + "/backup/global.cfg");

            // Without QFile::Text, so that the line terminator below is what ends up in
            // the file on every platform - the flag only produced CRLF on Windows.
            if (!global.open(QFile::WriteOnly))
            {
                // msg.fileOpenErrorCloseOMSI(parent, mapFolderPath); TODO
                qDebug().noquote() << "Cannot write file: Full path: '" + QFileInfo(global).absoluteFilePath() + "'";
                return FileIOResponse::errFileNotOpen;
            }

            QTextStream out(&global);

            // Give the file back in the shape it arrived in.
            out.setEncoding(fileFormat.encoding);
            out.setGenerateByteOrderMark(fileFormat.byteOrderMark);

            // OMSI stores coordinates with 15 significant digits. The default of six
            // truncated every one of them on each save and turned larger values into
            // scientific notation.
            out.setRealNumberPrecision(15);

            // OMSI and its editor write CRLF.
            const QString nl = "\r\n";

            // The file header and the description carry their own line breaks and are
            // kept with "\n" in memory, so they need converting too.
            const auto multiline = [&nl](QString text) { return text.replace("\n", nl); };

            try {
                out << multiline(OCBase::writeFileHeader()) << nl << nl;

                out << "[name]" << nl;
                out << name << nl << nl;

                out << "[friendlyname]" << nl;
                out << friendlyname << nl << nl;

                out << "[description]" << nl;
                out << multiline(description) << nl;
                out << "[end]" << nl << nl;

                out << "[version]" << nl;
                out << version << nl << nl;

                out << "[NextIDCode]" << nl;
                out << nextIDCode << nl << nl;

                if (worldCoodinates) out << "[worldcoordinates]" << nl << nl;

                if (dynHelpers) out << "[dynhelperactive]" << nl << nl;

                if (lht) out << "[LHT]" << nl << nl;

                if (realrail) out << "[realrail]" << nl << nl;

                out << "[backgroundimage]" << nl;
                out << bgImage.isVisible << nl;
                out << bgImage.picturePath.replace("/", "\\") << nl;
                out << bgImage.width << nl;
                out << bgImage.height << nl;
                out << bgImage.startWidth << nl;
                out << bgImage.startHeight << nl << nl;

                out << "[mapcam]" << nl;
                out << standardView.tile.y << nl;
                out << standardView.tile.x << nl;
                out << standardView.position.x << nl;
                out << standardView.position.z << nl;
                out << standardView.position.y << nl;
                out << standardView.rotAroundZ << nl;
                out << standardView.rotAroundX << nl;
                out << standardView.distanceFromPosition << nl << nl;

                out << "[moneysystem]" << nl;
                out << currency.replace("/", "\\") << nl << nl;

                out << "[ticketpack]" << nl;
                out << ticketpack.replace("/", "\\") << nl << nl;

                out << "[repair_time_min]" << nl;

                /*
                    Counterpart of the read above: minutes.

                    An invalid QTime - every map without this field - must not reach the
                    arithmetic. hour() returns -1 for it, and the previous loop
                    "while (hours != 0) hours--;" counted down from -1 and never ended.
                */
                out << (repairTime.isValid() ? repairTime.hour() * 60 + repairTime.minute() : 0) << nl << nl;

                out << "[years]" << nl;
                out << startYear << nl;
                out << endYear << nl << nl;

                out << "[standarddepot]" << nl;
                out << standardDepot << nl << nl;

                if (realYearOffset != 0)
                {
                    out << "[realyearoffset]" << nl;
                    out << realYearOffset << nl << nl;
                }

                for (int i = 0; i < groundTextures.count(); i++)
                {
                    out << "[groundtex]" << nl;
                    out << groundTextures[i].mainTex.replace("/", "\\") << nl;
                    out << groundTextures[i].subTex.replace("/", "\\") << nl;
                    out << groundTextures[i].texSizeExponent << nl;
                    out << groundTextures[i].mainTexRepeating << nl;
                    out << groundTextures[i].subTexRepeating << nl << nl;
                }

                seasons = sortSeasons(seasons);
                for (int i = 0; i < seasons.count(); i++)
                {
                    out << "[addseason]" << nl;
                    out << seasons[i].type << nl;
                    out << (QDate(QDate::currentDate().year(), 1, 1).daysTo(seasons[i].start)) << nl;
                    out << (QDate(QDate::currentDate().year(), 1, 1).daysTo(seasons[i].end)) << nl << nl;
                }

                std::sort(trafficDensities.begin(), trafficDensities.end(), [](const AiDensity &a, const AiDensity &b) {
                    return a.time < b.time;
                });

                for (int i = 0; i < trafficDensities.count(); i++)
                {
                    out << "[trafficdensity_road]" << nl;
                    // TODO: Workaround
                    if (trafficDensities[i].time == QTime(23, 59)) out << "24.000" << nl;
                    else out << QString::number(trafficDensities[i].time.hour()) + "." + QString::number(trafficDensities[i].time.minute() / 60) << nl;
                    out << trafficDensities[i].factor << nl << nl;
                }

                std::sort(passengerDensities.begin(), passengerDensities.end(), [](const AiDensity &a, const AiDensity &b) {
                    return a.time < b.time;
                });

                for (int i = 0; i < passengerDensities.count(); i++)
                {
                    out << "[trafficdensity_passenger]" << nl;
                    // TODO: Workaround
                    if (passengerDensities[i].time == QTime(23, 59)) out << "24.000" << nl;
                    else out << QString::number(passengerDensities[i].time.hour()) + "." + QString::number(passengerDensities[i].time.minute() / 60) << nl;
                    out << passengerDensities[i].factor << nl << nl;
                }

                out << "[entrypoints]" << nl;
                int count = 0;
                foreach (EntrypointCollection current, entrypoints) count += current.entrypoints.count();
                out << count << nl;
                for (int i = 0; i < entrypoints.count(); i++)
                {
                    for (int j = 0; j < entrypoints[i].entrypoints.count(); j++)
                    {
                        out << entrypoints[i].entrypoints[j].objectID << nl;
                        out << entrypoints[i].entrypoints[j].globalThingID << nl;
                        out << entrypoints[i].entrypoints[j].awkwardValue1 << nl;
                        out << entrypoints[i].entrypoints[j].position.x << nl;
                        out << entrypoints[i].entrypoints[j].position.z << nl;
                        out << entrypoints[i].entrypoints[j].position.y << nl;
                        out << entrypoints[i].entrypoints[j].awkwardValue2 << nl;
                        out << entrypoints[i].entrypoints[j].awkwardValue3 << nl;
                        out << entrypoints[i].entrypoints[j].awkwardValue4 << nl;
                        out << entrypoints[i].entrypoints[j].awkwardValue5 << nl;
                        out << entrypoints[i].entrypoints[j].tileID << nl;
                        out << entrypoints[i].name << nl;
                    }
                }

                out << nl;

                for (int i = 0; i < tiles.count(); i++)
                {
                    out << "[map]" << nl;
                    out << tiles[i].position.x << nl;
                    out << tiles[i].position.y << nl;
                    out << tiles[i].filename.replace("/", "\\") << nl << nl;
                }

                out << nl;

                global.close();
            }
            catch (...)
            {
                global.close();
                return FileIOResponse::errCritical;
            }

            return FileIOResponse::valid;
        }
    };

    class Tile // map
    {
    public:
        class Spline
        {
        public:
            class Rule
            {
            public:
                int pathIdent = -1; // TODO: std::optional?
                QString param; // TODO: params are const values, list them!
                int value = -1; // TODO: std::optional?
                int aiGroupIndex = -1; // TODO: std::optional?
            };

            bool h = false;

            //?
            QString path;
            //?...

            QList<Rule> rules;

            // Different param count spline / splineH?
        };

        class Sceneryobject
        {
        public:
            // TODO
            // possible for objects: [object] [splineAttachement] [attachObj] [splineAttachement_repeater]

            //?
            QString path;
            //?...

            std::optional<int> parentObjectID; // [varparent]

            // Attention: Different param count object / attachement!

            /* [object]
         * ?
         * path
         * objectID
         * xpos
         * ypos
         * zpos
         * rot
         * pitch
         * bank
         * labelCount
         * labelCount * labels... \./
        */
        };

        int version = 0;
        bool terrain = false;
        bool variableTerrainLightmap = false;
        bool variableTerrain = false;

        QList<Spline> splines;
        QList<Sceneryobject> sceneryobjects;
    };

    class UnschedTrafficDensitiesGroup // unsched_trafficdens.txt
    {
    public:
        class day
        {
        public:
            class density
            {
            public:
                float time = 0.0f;
                float density; // can be bigger as 1!
            };

            // 0 = all days - else: +1 = weekday, +2 = saturday, +4 = sunday
            int dayIdent = -1; // TODO: std::optional?
            QList<density> densities;

        };

        QString name;
        float density = 0.0f;
        QList<day> days;
    };

    class UnschedVehicleGroup // unsched_vehgroups.txt
    {
    public:
        QString name;
        /*
         * 0 = no traffic by default,
         * 1 = use density of first UVG by default,
         * 2 = ...            second ...          ,
         * 3 = ...
         if this is the first UVG, then 1 = use density class Medium by default */
        int defaultDensity = -1; // TODO: std::optional?
    };

    class Parklist
    {
    public:
        int ident = 0;
        QList<QString> objectList;
    };

    class Ailist
    {
    public:
        class Ailist1
        {
        public:
            class Group
            {
            public:
                QString name;
                int vehicleCount = 0;
                QList<int> vehicleIdents;
                QString hofFileName;
            };

            int vehicleClass = 0;
            int baseClassForUnschedAI = -1; // -1: All vehicles that weren't mentioned in a group
            int vehicleCount = 0;
            QList<QString> vehiclePaths;
            QList<Group> groups;
        };

        class Ailist2
        {
        public:
            class Group
            {
            public:
                class Vehicle
                {
                public:
                    QString vehiclePath;
                    int density = 0;
                };

                QString name;
                QString hofFileName; // TODO: ?
                QList<Vehicle> vehicles;
            };

            class Groupdepot
            {
            public:
                class Typgroup2
                {
                public:
                    class Vehicle
                    {
                    public:
                        std::optional<QString> number;
                        std::optional<QString> repaintName;
                        std::optional<QString> regPlate;
                        std::optional<QDate> startDate;
                        std::optional<QDate> endDate;
                    };

                    QString vehiclePath;
                    QList<Vehicle> vehicles;
                };

                QString name;
                QString hofFileName;

                // TODO: Typgroup (Typgroup1)?
                QList<Typgroup2> typgroup2s;
            };

            QList<Group> groups;
            QList<Groupdepot> groupdepots;
        };

        bool useAilist2; // TODO: (?), correct?
    };

    class Chrono
    {
    public:
        class Tile
        {
        public:
            class SelectObject
            {
            public:
                int ident = -1;

                QList<QString> relabels;

                bool isDeleted = false;
                QString type; // optional new path for an object
            };

            class SelectSpline
            {
            public:
                int ident = -1;

                QList<OCMap::Map::Tile::Spline::Rule> rules;

                bool isDeleted = false;
                QString type; // optional new path for an spline
            };

            int version = 0;

            QList<OCMap::Map::Tile::Spline> splines;
            QList<OCMap::Map::Tile::Sceneryobject> sceneryobjects;
        };

        QString folderName;
        QString name;
        QString description;
        QList<QString> deactivatedLines;
        QDate start;
        QDate end;
        QString ticketpack;
        QString moneysystem;
        QList<Tile> tiles;
        bool hasAilist; // ailists_#upd.txt
        bool hasAilistLow; // ailists_#low_#upd.txt

        // TODO: TTData!
        // TODO: humans, drivers etc.?
    };

    class Signalroute
    {
    public:
            // TODO
    };

    class Holidays
    {
        // TODO: This file exists multiple times with file (pre-)extension [language code].
    public:
        QDate start;
        QDate end;
        QString name;
    };

    class Holiday
    {
        // TODO: This file exists multiple times with file (pre-)extension [language code].
    public:
        QDate date;
        QString name;
    };

    class Timezone
    {
    public:
        class DaylightSavingTime
        {
        public:
            QDate start; // with hours!
            QDate end; // with hours!
            int changeHours = 0;
        };

        int timezone = 0;
        OCType::Coord2D<float> location;

        QList<DaylightSavingTime> daylistSavingTimes;
    };

    class CarUse
    {
    public:
        class TypeTour
        {
        public:
            QString vehicleIdent; // number or path to vehicle file
            QString tour;

            // if (typesPrefered) {
            float probability = 0.0f;
            // }
        };

        QString filename;

        QDate start;
        QDate end;

        QString line;
        QList<TypeTour> numberTours;
        QList<TypeTour> typeTours;
        QList<TypeTour> typesPrefered;
        QList<QString> onlyTypes;

    };

    class TTData
    {
    public:
        class Trip
        {
        public:
            class Station
            {
            public:
                int objectID = 0;
                int trackIndexParent = 0;
                QString name;
                int tileID = 0;
                float awkwardValue1 = 0.0f;
                float awkwardValue2 = 0.0f;
                float awkwardValue3 = 0.0f;
                float awkwardValue4 = 0.0f;
            };

            class Profile
            {
            public:
                QString name;
                QTime duration; // TODO: good type for use-cases?
            };

            QString name;
            QString trackName;
            QString terminus;
            QString line;

            QList<Station> stations;
            QList<Profile> profiles;
        };

        class Tour
        {
        public:
            class TripInformation
            {
            public:
                QString name;
                int profileIndex = 0;
                QTime start;
            };

            enum Day
            {
                Monday = 1,
                Tuesday = 2,
                Wednesday = 4,
                Thursday = 8,
                Friday = 16,
                Saturday = 32,
                Sunday = 64,
                Holiday = 128,
                Hols = 256,
                NoHols = 512
            };

            Q_DECLARE_FLAGS(Days, Day)

            QString name;
            QString aiGroupName;
            Days days;
            QList<TripInformation> trips;
        };

        class Line
        {
        public:
            QString name;
            bool userAllowed = false;
            unsigned int priority = 0;

            QList<Tour> tours;
        };

        QList<Trip> trips;
        QList<Line> lines;

        Global::FileIOResponse read()
        {
            clear();

            QDirIterator dirIteratorTtp(OTPath::resolve(dir, "TTData"), QStringList() << "*.ttp", QDir::Files, QDirIterator::Subdirectories);

            // Trips
            while (dirIteratorTtp.hasNext())
            {
                QFile ttp(dirIteratorTtp.next());
                if (!ttp.open(QFile::ReadOnly | QFile::Text))
                {
                    // msg.fileOpenErrorCloseOMSI(parent, dir); TODO
                    qDebug().noquote() << "Cannot open file: Full path: '" + QFileInfo(ttp).absoluteFilePath() + "'";
                    return Global::FileIOResponse::errFileNotOpen;
                }

                Trip trip;
                trip.name = QFileInfo(ttp).baseName();

                QTextStream in(&ttp);
                in.setEncoding(QStringConverter::Latin1);
                QString line = "";

                try
                {
                    while (!in.atEnd())
                    {
                        line = in.readLine();

                        if (line == "[trip]")
                        {
                            trip.trackName = in.readLine();
                            trip.terminus = in.readLine();
                            trip.line = in.readLine();
                        }
                        else if (line == "[station]")
                        {
                            Trip::Station station;
                            station.objectID = in.readLine().toInt();
                            station.trackIndexParent = in.readLine().toInt();
                            station.name = in.readLine();
                            station.tileID = in.readLine().toInt();
                            station.awkwardValue1 = in.readLine().toDouble();
                            station.awkwardValue2 = in.readLine().toDouble();
                            station.awkwardValue3 = in.readLine().toDouble();
                            station.awkwardValue4 = in.readLine().toDouble();

                            trip.stations << station;
                        }
                        else if (line == "[profile]")
                        {
                            Trip::Profile profile;
                            profile.name = in.readLine();
                            profile.duration = QTime::fromMSecsSinceStartOfDay(in.readLine().toDouble() * 60 * 1000);

                            trip.profiles << profile;
                        }
                    }

                    ttp.close();

                    trips << trip;
                }
                catch (...)
                {
                    ttp.close();
                    return Global::FileIOResponse::errCritical;
                }
            }

            QDirIterator dirIteratorTtl(OTPath::resolve(dir, "TTData"), QStringList() << "*.ttl", QDir::Files, QDirIterator::Subdirectories);

            // Lines & Tours
            while (dirIteratorTtl.hasNext())
            {
                QFile ttl(dirIteratorTtl.next());
                if (!ttl.open(QFile::ReadOnly | QFile::Text))
                {
                    // msg.fileOpenErrorCloseOMSI(parent, dir); TODO
                    qDebug().noquote() << "Cannot open file: Full path: '" + QFileInfo(ttl).absoluteFilePath() + "'";
                    return Global::FileIOResponse::errFileNotOpen;
                }

                Line lineTT;
                lineTT.name = QFileInfo(ttl).baseName();

                QTextStream in(&ttl);
                in.setEncoding(QStringConverter::Latin1);
                QString line = "";

                try
                {
                    while (!in.atEnd())
                    {
                        line = in.readLine();

                        if (line == "[userallowed]") lineTT.userAllowed = true;
                        else if (line == "[priority]") lineTT.priority = in.readLine().toInt();
                        else if (line == "[newtour]")
                        {
                            Tour tour;
                            tour.name = in.readLine();
                            tour.aiGroupName = in.readLine();

                            int dayValue = in.readLine().toInt();

                            while (true)
                            {
                                if      (dayValue >= 512) { tour.days |= Tour::Hols;      dayValue -= 512; }
                                else if (dayValue >= 256) { tour.days |= Tour::NoHols;    dayValue -= 256; }
                                else if (dayValue >= 128) { tour.days |= Tour::Holiday;   dayValue -= 128; }
                                else if (dayValue >= 64)  { tour.days |= Tour::Sunday;    dayValue -= 64; }
                                else if (dayValue >= 32)  { tour.days |= Tour::Saturday;  dayValue -= 32; }
                                else if (dayValue >= 16)  { tour.days |= Tour::Friday;    dayValue -= 16; }
                                else if (dayValue >= 8)   { tour.days |= Tour::Thursday;  dayValue -= 8; }
                                else if (dayValue >= 4)   { tour.days |= Tour::Wednesday; dayValue -= 4; }
                                else if (dayValue >= 2)   { tour.days |= Tour::Tuesday;   dayValue -= 2; }
                                else if (dayValue >= 1)   { tour.days |= Tour::Monday;    break; }
                                else break;
                            }

                            lineTT.tours << tour;
                        }
                        else if (line == "[addtrip]")
                        {
                            Tour::TripInformation tripInformation;

                            tripInformation.name = in.readLine();
                            tripInformation.profileIndex = in.readLine().toInt();
                            tripInformation.start = QTime::fromMSecsSinceStartOfDay(in.readLine().toDouble() * 60 * 1000);

                            lineTT.tours.last().trips << tripInformation;
                        }
                    }

                    ttl.close();

                    lines << lineTT;
                }
                catch (...)
                {
                    ttl.close();
                    return Global::FileIOResponse::errCritical;
                }
            }

            return Global::FileIOResponse::valid;
        }

        Global::FileIOResponse write() {
            // TODO
            return Global::FileIOResponse::errFunctionNotDefined;
        }

        void clear() {
            trips.clear();
            lines.clear();
        }
    };

    Global global;
    Timezone timezone;
    TTData ttData;
    QList<CarUse> carUses;
    QList<QString> humans; // humans.txt
    QList<QString> drivers; // drivers.txt
    QList<QString> registrations; // regstrations.txt
    QList<Parklist> parklists; // parklist.txt | parklist_n.txt
    QList<Chrono> chronos; // \Chrono\*[DIR]
    QList<UnschedVehicleGroup> unschedVehicleGroups; // unsched_vehgroups.txt
    QList<UnschedTrafficDensitiesGroup> unschedTrafficDensitiesGroups; // unsched_trafficdens.txt
    QList<Signalroute> signalroutes; // signalroutes.cfg
    QList<Holidays> holidaysMultiple; // holidays.txt | holidays_[LGC].txt // TODO: see list class
    QList<Holiday> holidaysSingle; // holidays.txt | holidays_[LGC].txt // TODO: see list class
    bool hasAilist; // ailists.txt
    bool hasAilistLow; // ailists_#low.txt

    inline static QString dir = "";
};

}

Q_DECLARE_OPERATORS_FOR_FLAGS(OCMap::Map::TTData::Tour::Days);

#endif // OCMAP_H
