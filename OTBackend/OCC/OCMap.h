#ifndef OCMAP_H
#define OCMAP_H

#include "OCBase.h"

class OCMapPosition
{
public:
    OCType::Coord2D<int> tile;

    OCType::Coord3D<float> position;

    float rotAroundZ;

    float rotAroundX;

    float distanceFromPosition;
};

class OCMap { // TODO: override clear function
public:
    class Global : public OCBase::File // global.cfg
    {
    public:
        class Texture
        {
        public:
            QString mainTex;

            QString subTex;

            int texSizeExponent;

            int mainTexRepeating;

            int subTexRepeating;
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

            float factor;
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

                int objectID;

                int globalThingID;

                int awkwardValue1;

                OCType::Coord3D<float> position;

                float awkwardValue2;

                float awkwardValue3;

                float awkwardValue4;

                float awkwardValue5;

                int tileID;
            };
            QString name;

            QList<Entrypoint> entrypoints;
        };

        class BackgroundImage
        {
        public:
            bool isVisible;

            QString picturePath;

            float width;

            float height;

            float startWidth;

            float startHeight;
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

        unsigned int nextIDCode;

        bool worldCoodinates;

        bool dynHelpers;

        bool lht;

        bool realrail;

        BackgroundImage bgImage;

        OCMapPosition standardView;

        QString currency;

        QString ticketpack;

        QTime repairTime;

        int startYear;

        int endYear;

        int realYearOffset;

        QString standardDepot;

        QList<Texture> groundTextures;

        QList<Season> seasons;

        QList<AiDensity> trafficDensities;

        QList<AiDensity> passengerDensities;

        QList<EntrypointCollection> entrypoints;

        QList<TileInformation> tiles;

        FileIOResponse read() override
        {
            if (dir.isEmpty())
            {
                qWarning() << "Reading process of Global stopped: No file path given.";
                return FileIOResponse::errFileDoesntExist;
            }

            QFile global(dir + "/global.cfg");

            if (!global.open(QFile::ReadOnly | QFile::Text))
            {
                // msg.fileOpenErrorCloseOMSI(parent, mapFolderPath); TODO
                qDebug().noquote() << "Cannot open file: Full path: '" + QFileInfo(global).absoluteFilePath() + "'";
                return FileIOResponse::errFileNotOpen;
            }

            QTextStream in(&global);
            in.setEncoding(QStringConverter::Latin1);
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

                        description.replace("Ä", "Ae", Qt::CaseSensitive);
                        description.replace("Ö", "Oe", Qt::CaseSensitive);
                        description.replace("Ü", "Ue", Qt::CaseSensitive);
                        description.replace("ä", "ae", Qt::CaseSensitive);
                        description.replace("ö", "oe", Qt::CaseSensitive);
                        description.replace("ü", "ue", Qt::CaseSensitive);
                        description.replace("ß", "ss", Qt::CaseSensitive);

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
                        bgImage.width = in.readLine().toFloat(); // 3
                        bgImage.height = in.readLine().toFloat(); // 4
                        bgImage.startWidth = in.readLine().toFloat(); // 5
                        bgImage.startHeight = in.readLine().toFloat(); // 6
                    }
                    else if (line == "[mapcam]")
                    {
                        // Attention: Inverted values
                        int yTilePos = in.readLine().toInt();
                        int xTilePos = in.readLine().toInt();

                        standardView.tile = OCType::Coord2D<int>(xTilePos, yTilePos); // 1, 2

                        // Attention: Inverted values
                        float xPos = in.readLine().toFloat(); // 3
                        float zPos = in.readLine().toFloat(); // 4
                        float yPos = in.readLine().toFloat(); // 5
                        standardView.position = OCType::Coord3D<float>(xPos, yPos, zPos); // 3, 4, 5

                        standardView.rotAroundZ = in.readLine().toFloat();
                        standardView.rotAroundX = in.readLine().toFloat();
                        standardView.distanceFromPosition = in.readLine().toFloat();
                    }
                    else if (line == "[moneysystem]") currency = in.readLine();
                    else if (line == "[ticketpack]") ticketpack = in.readLine();
                    else if (line == "[repair_time_min]")
                    {
                        float decimalTime = in.readLine().toFloat(); int hours = 0;
                        while (decimalTime >= 60) { hours++; decimalTime -= 60; }
                        repairTime = QTime(hours, decimalTime * 60);
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
                        float decimalTime = in.readLine().toFloat(); int hours = static_cast<int>(decimalTime);
                        // TODO: Workaround
                        density.time = (hours != 24) ? QTime(hours, (decimalTime - hours) * 60) : QTime(23, 59);
                        density.factor = in.readLine().toFloat();
                        trafficDensities.append(density);
                    }
                    else if (line == "[trafficdensity_passenger]")
                    {
                        AiDensity density;
                        float decimalTime = in.readLine().toFloat(); int hours = static_cast<int>(decimalTime);
                        // TODO: Workaround
                        density.time = (hours != 24) ? QTime(hours, (decimalTime - hours) * 60) : QTime(23, 59);
                        density.factor = in.readLine().toFloat();
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
                            float xPos = in.readLine().toFloat();
                            float zPos = in.readLine().toFloat();
                            float yPos = in.readLine().toFloat();
                            entrypoint.position = OCType::Coord3D<float>(xPos, yPos, zPos);

                            entrypoint.awkwardValue2 = in.readLine().toFloat();
                            entrypoint.awkwardValue3 = in.readLine().toFloat();
                            entrypoint.awkwardValue4 = in.readLine().toFloat();
                            entrypoint.awkwardValue5 = in.readLine().toFloat();
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

            QFile global(dir + "/global.cfg");

            // Backup
            if (!QDir().exists(dir + "/backup")) qDebug() << "Backup dir create:" << QDir().mkdir(dir + "/backup");
            if (QFile(dir + "/backup/global.cfg").exists()) QFile(dir + "/backup/global.cfg").remove();
            global.copy(dir + "/backup/global.cfg");

            if (!global.open(QFile::WriteOnly | QFile::Text))
            {
                // msg.fileOpenErrorCloseOMSI(parent, mapFolderPath); TODO
                qDebug().noquote() << "Cannot write file: Full path: '" + QFileInfo(global).absoluteFilePath() + "'";
                return FileIOResponse::errFileNotOpen;
            }

            QTextStream out(&global);
            // Notepad++ -> UTF-8 - readable by OMSI!
            out.setEncoding(QStringConverter::Latin1);

            try {
                out << OCBase::writeFileHeader() << "\n\n";

                out << "[name]" << "\n";
                out << name << "\n\n";

                out << "[friendlyname]" << "\n";
                out << friendlyname << "\n\n";

                out << "[description]" << "\n";
                out << description << "\n";
                out << "[end]" << "\n\n";

                out << "[version]" << "\n";
                out << version << "\n\n";

                out << "[NextIDCode]" << "\n";
                out << nextIDCode << "\n\n";

                if (worldCoodinates) out << "[worldcoordinates]" << "\n\n";

                if (dynHelpers) out << "[dynhelperactive]" << "\n\n";

                if (lht) out << "[LHT]" << "\n\n";

                if (realrail) out << "[realrail]" << "\n\n";

                out << "[backgroundimage]" << "\n";
                out << bgImage.isVisible << "\n";
                out << bgImage.picturePath.replace("/", "\\") << "\n";
                out << bgImage.width << "\n";
                out << bgImage.height << "\n";
                out << bgImage.startWidth << "\n";
                out << bgImage.startHeight << "\n\n";

                out << "[mapcam]" << "\n";
                out << standardView.tile.y << "\n";
                out << standardView.tile.x << "\n";
                out << standardView.position.x << "\n";
                out << standardView.position.z << "\n";
                out << standardView.position.y << "\n";
                out << standardView.rotAroundZ << "\n";
                out << standardView.rotAroundX << "\n";
                out << standardView.distanceFromPosition << "\n\n";

                out << "[moneysystem]" << "\n";
                out << currency.replace("/", "\\") << "\n\n";

                out << "[ticketpack]" << "\n";
                out << ticketpack.replace("/", "\\") << "\n\n";

                out << "[repair_time_min]" << "\n";
                float decimalRepairTime = 0; int hours = repairTime.hour();
                while (hours != 0) { hours--; decimalRepairTime += 60; }
                decimalRepairTime += repairTime.minute() / 60;
                out << decimalRepairTime << "\n\n";

                out << "[years]" << "\n";
                out << startYear << "\n";
                out << endYear << "\n\n";

                out << "[standarddepot]" << "\n";
                out << standardDepot << "\n\n";

                if (realYearOffset != 0)
                {
                    out << "[realyearoffset]" << "\n";
                    out << realYearOffset << "\n\n";
                }

                for (int i = 0; i < groundTextures.count(); i++)
                {
                    out << "[groundtex]" << "\n";
                    out << groundTextures[i].mainTex.replace("/", "\\") << "\n";
                    out << groundTextures[i].subTex.replace("/", "\\") << "\n";
                    out << groundTextures[i].texSizeExponent << "\n";
                    out << groundTextures[i].mainTexRepeating << "\n";
                    out << groundTextures[i].subTexRepeating << "\n\n";
                }

                seasons = sortSeasons(seasons);
                for (int i = 0; i < seasons.count(); i++)
                {
                    out << "[addseason]" << "\n";
                    out << seasons[i].type << "\n";
                    out << (QDate(QDate::currentDate().year(), 1, 1).daysTo(seasons[i].start)) << "\n";
                    out << (QDate(QDate::currentDate().year(), 1, 1).daysTo(seasons[i].end)) << "\n\n";
                }

                std::sort(trafficDensities.begin(), trafficDensities.end(), [](const AiDensity &a, const AiDensity &b) {
                    return a.time < b.time;
                });

                for (int i = 0; i < trafficDensities.count(); i++)
                {
                    out << "[trafficdensity_road]" << "\n";
                    // TODO: Workaround
                    if (trafficDensities[i].time == QTime(23, 59)) out << "24.000\n";
                    else out << QString::number(trafficDensities[i].time.hour()) + "." + QString::number(trafficDensities[i].time.minute() / 60) << "\n";
                    out << trafficDensities[i].factor << "\n\n";
                }

                std::sort(passengerDensities.begin(), passengerDensities.end(), [](const AiDensity &a, const AiDensity &b) {
                    return a.time < b.time;
                });

                for (int i = 0; i < passengerDensities.count(); i++)
                {
                    out << "[trafficdensity_passenger]" << "\n";
                    // TODO: Workaround
                    if (passengerDensities[i].time == QTime(23, 59)) out << "24.000\n";
                    else out << QString::number(passengerDensities[i].time.hour()) + "." + QString::number(passengerDensities[i].time.minute() / 60) << "\n";
                    out << passengerDensities[i].factor << "\n\n";
                }

                out << "[entrypoints]" << "\n";
                int count = 0;
                foreach (EntrypointCollection current, entrypoints) count += current.entrypoints.count();
                out << count << "\n";
                for (int i = 0; i < entrypoints.count(); i++)
                {
                    for (int j = 0; j < entrypoints[i].entrypoints.count(); j++)
                    {
                        out << entrypoints[i].entrypoints[j].objectID << "\n";
                        out << entrypoints[i].entrypoints[j].globalThingID << "\n";
                        out << entrypoints[i].entrypoints[j].awkwardValue1 << "\n";
                        out << entrypoints[i].entrypoints[j].position.x << "\n";
                        out << entrypoints[i].entrypoints[j].position.z << "\n";
                        out << entrypoints[i].entrypoints[j].position.y << "\n";
                        out << entrypoints[i].entrypoints[j].awkwardValue2 << "\n";
                        out << entrypoints[i].entrypoints[j].awkwardValue3 << "\n";
                        out << entrypoints[i].entrypoints[j].awkwardValue4 << "\n";
                        out << entrypoints[i].entrypoints[j].awkwardValue5 << "\n";
                        out << entrypoints[i].entrypoints[j].tileID << "\n";
                        out << entrypoints[i].name << "\n";
                    }
                }

                out << "\n";

                for (int i = 0; i < tiles.count(); i++)
                {
                    out << "[map]" << "\n";
                    out << tiles[i].position.x << "\n";
                    out << tiles[i].position.y << "\n";
                    out << tiles[i].filename.replace("/", "\\") << "\n\n";
                }

                out << "\n";

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

    class Tile : public OCBase::File // map
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

            bool h;

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

        int version;
        bool terrain;
        bool variableTerrainLightmap;
        bool variableTerrain;

        QList<Spline> splines;
        QList<Sceneryobject> sceneryobjects;
    };

    class UnschedTrafficDensitiesGroup : public OCBase::File // unsched_trafficdens.txt
    {
    public:
        class day
        {
        public:
            class density
            {
            public:
                float time;
                float density; // can be bigger as 1!
            };

            // 0 = all days - else: +1 = weekday, +2 = saturday, +4 = sunday
            int dayIdent = -1; // TODO: std::optional?
            QList<density> densities;

        };

        QString name;
        float density;
        QList<day> days;
    };

    class UnschedVehicleGroup : public OCBase::File // unsched_vehgroups.txt
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
        int ident;
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
                int vehicleCount;
                QList<int> vehicleIdents;
                QString hofFileName;
            };

            int vehicleClass;
            int baseClassForUnschedAI = -1; // -1: All vehicles that weren't mentioned in a group
            int vehicleCount;
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
                    int density;
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

                bool isDeleted;
                QString type; // optional new path for an object
            };

            class SelectSpline
            {
            public:
                int ident = -1;

                QList<OCMap::Tile::Spline::Rule> rules;

                bool isDeleted;
                QString type; // optional new path for an spline
            };

            int version;

            QList<OCMap::Tile::Spline> splines;
            QList<OCMap::Tile::Sceneryobject> sceneryobjects;
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
            int changeHours;
        };

        int timezone;
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
            float probability;
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

    class TTData : public OCBase::File
    {
    public:
        class Trip
        {
        public:
            class Station
            {
            public:
                int objectID;
                int trackIndexParent;
                QString name;
                int tileID;
                float awkwardValue1;
                float awkwardValue2;
                float awkwardValue3;
                float awkwardValue4;
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
                int profileIndex;
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
            unsigned int priority;

            QList<Tour> tours;
        };

        QList<Trip> trips;
        QList<Line> lines;

        Global::FileIOResponse read()
        {
            clear();

            QDirIterator dirIteratorTtp(dir + "/TTData", QStringList() << "*.ttp", QDir::Files, QDirIterator::Subdirectories);

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
                            station.awkwardValue1 = in.readLine().toFloat();
                            station.awkwardValue2 = in.readLine().toFloat();
                            station.awkwardValue3 = in.readLine().toFloat();
                            station.awkwardValue4 = in.readLine().toFloat();

                            trip.stations << station;
                        }
                        else if (line == "[profile]")
                        {
                            Trip::Profile profile;
                            profile.name = in.readLine();
                            profile.duration = QTime::fromMSecsSinceStartOfDay(in.readLine().toFloat() * 60 * 1000);

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

            QDirIterator dirIteratorTtl(dir + "/TTData", QStringList() << "*.ttl", QDir::Files, QDirIterator::Subdirectories);

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
                            tripInformation.start = QTime::fromMSecsSinceStartOfDay(in.readLine().toFloat() * 60 * 1000);

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

Q_DECLARE_OPERATORS_FOR_FLAGS(OCMap::TTData::Tour::Days);

#endif // OCMAP_H
