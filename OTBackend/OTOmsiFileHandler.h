#ifndef OTOMSIFILEHANDLER_H
#define OTOMSIFILEHANDLER_H
#include "OTGlobal.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QApplication>

class OTVerifyStuff
{
public:
    class OTVerifyStuffMissing
    {
    public:
        QStringList tiles;
        QStringList textures;
        QStringList globalTextures;
        QStringList sceneryobjects;
        QStringList splines;
        QStringList vehicles;
        QStringList humans;

        QStringList getAll()
        {
            QStringList all;
            all << tiles;
            all << textures;
            all << globalTextures;
            all << sceneryobjects;
            all << splines;
            all << vehicles;
            all << humans;

            return all;
        }

        void removeDuplicates()
        {
            tiles.removeDuplicates();
            textures.removeDuplicates();
            globalTextures.removeDuplicates();
            sceneryobjects.removeDuplicates();
            splines.removeDuplicates();
            vehicles.removeDuplicates();
            humans.removeDuplicates();
        }

        void clear()
        {
            tiles.clear();
            textures.clear();
            globalTextures.clear();
            sceneryobjects.clear();
            splines.clear();
            vehicles.clear();
            humans.clear();
        }
    };

    class OTVerifyStuffExisting
    {
    public:
        QStringList tiles;
        QStringList textures;
        QStringList globalTextures;
        QStringList sceneryobjects;
        QStringList splines;
        QStringList vehicles;
        QStringList humans;

        QStringList getAll()
        {
            QStringList all;
            all << tiles;
            all << textures;
            all << globalTextures;
            all << sceneryobjects;
            all << splines;
            all << vehicles;
            all << humans;

            return all;
        }

        void removeDuplicates()
        {
            tiles.removeDuplicates();
            textures.removeDuplicates();
            globalTextures.removeDuplicates();
            sceneryobjects.removeDuplicates();
            splines.removeDuplicates();
            vehicles.removeDuplicates();
            humans.removeDuplicates();
        }

        void clear()
        {
            tiles.clear();
            textures.clear();
            globalTextures.clear();
            sceneryobjects.clear();
            splines.clear();
            vehicles.clear();
            humans.clear();
        }
    };

    OTVerifyStuffMissing missing;
    OTVerifyStuffExisting existing;

    void clear()
    {
        missing.clear();
        existing.clear();
    }

    void removeDuplicates()
    {
        missing.removeDuplicates();
        existing.removeDuplicates();
    }
};

/// Contains all informations about a font character
class OTCharacterModel
{
public:
    OTCharacterModel(QString ca = "", int lP = -1, int rP = -1, int hP = -1, QString co = "")
    {
        character = ca;
        leftPixel = lP;
        rightPixel = rP;
        highestPixelInFontRow = hP;
        comment = co;
    }

    QString character;
    int leftPixel;
    int rightPixel;
    int highestPixelInFontRow;
    QString comment;
};

/// Contains all informations about a font
class OTFontModel
{
public:
    bool error = false;
    bool moreThanOneFont = false;

    QString path;
    QString name;
    QString colorTexture;
    QString alphaTexture;
    int maxHeightOfChars = -1;
    int distanceBetweenChars = -1;

    QList<OTCharacterModel> charList;

    void clear()
    {
        error = false;
        path.clear();
        name.clear();
        colorTexture.clear();
        alphaTexture.clear();
        maxHeightOfChars = int();
        distanceBetweenChars = int();
        charList.clear();
    }
};

class OTOMSIFileHandler
{
public:
    OTVerifyStuff stuffobj;

    OTOMSIFileHandler()
    {
        preDefiniedLocalVariables << "NIGHTLIGHTA";
        preDefiniedLocalVariables << "INUSE";
        preDefiniedLocalVariables << "TRAFFICLIGHTPHASE";
        preDefiniedLocalVariables << "TRAFFICLIGHTAPPROACH";
        preDefiniedLocalVariables << "COLORSCHEME";
        preDefiniedLocalVariables << "SIGNAL";
        preDefiniedLocalVariables << "NEXTSIGNAL";
        preDefiniedLocalVariables << "REFRESH_STRINGS";
        preDefiniedLocalVariables << "SWITCH";
    }

    int maxProgress = 0;
    unsigned int currentProgress = 0;
    QString progressName;

    QStringList preDefiniedLocalVariables;

    /// Returns a QStringList with all variables - needs an absoulute path
    QStringList readVarlist(QString path)
    {
        cutCount = set.read("main", "mainDir").toString().count() + 1;

        QStringList variables;
        QFile varnamelist(path);
        if (varnamelist.open (QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&varnamelist);
            in.setEncoding(QStringConverter::System);
            while (!in.atEnd())
                variables << in.readLine().toUpper();
        }
        varnamelist.close();

        return variables;
    }

    /// verify a list of sceneryobjects
    void verifyObjects(QStringList &objects)
    {
        cutCount = set.read("main", "mainDir").toString().count() + 1;

        progressName = QObject::tr("Checking sceneryobjects...");
        maxProgress = objects.count();
        int i = 0;

        foreach (QString current, objects)
        {
            i++;
            currentProgress = i;
            qApp->processEvents();

//            if (current.contains("Aufzug.sco"))
//                qDebug().noquote() << "Here is " << current << "!";

            QFile object(set.read("main", "mainDir").toString() + "/" + current);
            object.open(QFile::ReadOnly | QFile::Text);

            if (object.exists())
            {
                QTextStream inFirst(&object);
                inFirst.setEncoding(QStringConverter::System);

                QString line;
                QStringList variables;

                QStringList stringvariables;
                QString currentDir = QDir(QFileInfo(object).dir()).absolutePath();

                QStringList textTextures;
                QString currentTexture;

                // At first, collect varnames:
                while (!inFirst.atEnd())
                {
                    line = inFirst.readLine();

                    // varnamelists:
                    if (line == "[varnamelist]")
                    {
                        QStringList varnameFiles;
                        int count = inFirst.readLine().toInt();
                        for (int i = 0; i < count; i++)
                            varnameFiles << inFirst.readLine();

                        foreach (QString currentVarlist, varnameFiles)
                        {
                            QString varlist = currentDir + "/" + currentVarlist;

                            while (varlist.at(varlist.count() - 1) == ' ')
                                varlist.remove(varlist.length() - 1, 1);

                            if (!QFile(varlist).exists())
                            {
                                qWarning().noquote() << "Error in object '" + current + "':" << "Varlist '" + currentVarlist + "' could not be found!";
                                stuffobj.missing.sceneryobjects << current;
                            }
                            else
                                variables = readVarlist(varlist);
                        }
                    }

                    // stringvarnamelists:
                    else if (line == "[stringvarnamelist]")
                    {
                        QStringList stringvarnameFiles;
                        int count = inFirst.readLine().toInt();
                        for (int i = 0; i < count; i++)
                            stringvarnameFiles << inFirst.readLine();

                        foreach (QString currentVarlist, stringvarnameFiles)
                        {
                            QString stringvarlist = currentDir + "/" + currentVarlist;

                            while (stringvarlist.at(stringvarlist.count() - 1) == ' ')
                                stringvarlist.remove(stringvarlist.length() - 1, 1);

                            if (!QFile(stringvarlist).exists())
                            {
                                qWarning().noquote() << "Error in object '" + current + "':" << "Stringvarlist '" + currentVarlist + "' could not be found!";
                                stuffobj.missing.sceneryobjects << current;
                            }
                            else
                                stringvariables = readVarlist(stringvarlist);
                        }
                    }

                    // matl for textTextures
                    else if (line == "[matl]")
                    {
                        line = inFirst.readLine();
                        currentTexture = line;
                    }

                    // useTextTexture
                    else if (line == "[useTextTexture]")
                        textTextures << currentTexture;
                }

                textTextures.removeDuplicates();

                object.close();
                object.open(QFile::ReadOnly | QFile::Text);

                QTextStream in(&object);
                in.setEncoding(QStringConverter::System);

                while (!in.atEnd())
                {
                    line = in.readLine();

                    // visible:
                    if (line == "[visible]")
                    {
                        line = in.readLine();

                        while (line.at(line.count() - 1) == ' ')
                            line.remove(line.length() - 1, 1);

                        // Check if varname exists
                        if (!variables.contains(line.toUpper()) && !preDefiniedLocalVariables.contains(line.toUpper()))
                        {
                            qWarning().noquote() << "Error in object '" + current + "':" << "[visible] variable '" + line + "' could not be found!";
                            qDebug().noquote() << "In List:" << variables << "(+ local pre-defined variables)";
                            stuffobj.missing.sceneryobjects << current;
                        }
                    }

                    // scripts:
                    else if (line == "[script]")
                    {
                        for (int i = 0; i < in.readLine().toInt(); i++)
                        {
                            QString scriptPath = in.readLine();
                            while (scriptPath.at(scriptPath.count() - 1) == ' ')
                                scriptPath.remove(scriptPath.length() - 1, 1);

                            if (!QFile(currentDir + "/" + scriptPath).exists())
                            {
                                qWarning().noquote() << "Error in object '" + current + "':" << "Script '" + scriptPath + "' could not be found!";
                                stuffobj.missing.sceneryobjects << current;
                            }
                        }
                    }

                    // constfiles:
                    else if (line == "[constfile]")
                    {
                        for (int i = 0; i < in.readLine().toInt(); i++)
                        {
                            QString constfilePath = in.readLine();
                            while (constfilePath.at(constfilePath.count() - 1) == ' ')
                                constfilePath.remove(constfilePath.length() - 1, 1);

                            if (!QFile(currentDir + "/" + constfilePath).exists())
                            {
                                qWarning().noquote() << "Error in object '" + current + "':" << "Constfile '" + constfilePath + "' could not be found!";
                                stuffobj.missing.sceneryobjects << current;
                            }
                        }
                    }

                    // Meshes:
                    else if (line == "[mesh]" || line == "[collision_mesh]" || line == "[crossing_heightdeformation]" || line == "[terrainhole]")
                    {
                        line = in.readLine();
                        while (line.at(line.count() - 1) == ' ')
                            line.remove(line.length() - 1, 1);

                        QString fullPath = QDir(QFileInfo(object).dir()).absolutePath() + "/model/" + line;
                        if (!QFile(fullPath).exists())
                        {
                            qWarning().noquote() << "Error in object '" + current + "':" << "Mesh '" + line + "' could not be found!";
                            stuffobj.missing.sceneryobjects << current;
                        }
                    }

                    // Materials:
                    else if (line == "[matl]" || line == "[matl_nightmap]" || line == "[matl_lightmap]" || line == "[matl_envmap]" || line == "[matl_envmap_mask]")
                    {
                        line = in.readLine();

                        while (line.at(line.count() - 1) == ' ')
                            line.remove(line.length() - 1, 1);

                        if (!textTextures.contains(line))
                        {
                            QString path = QFileInfo(QFileInfo(object).dir().absolutePath() + "/texture/" + line).absoluteFilePath().remove(0, cutCount);

                            if (!checkTexture(QDir(QFileInfo(object).dir()).absolutePath() + "/texture/" + line, line))
                            {
                                qWarning().noquote() << "Error in object '" + current + "':" << "Texture '" + line + "' could not be found!";
                                // old: path
                                stuffobj.missing.textures << QDir(QFileInfo(object).dir()).absolutePath() + "/texture/" + line;
                                qDebug().noquote() << "ABSOLUTE PATH:" << QFileInfo(QFileInfo(object).dir().absolutePath() + "/texture/" + line).absoluteFilePath();
                                qDebug() << "\n----------------------------------------------------------------------------------------------";
                            }
                            else
                                stuffobj.existing.textures << path;
                        }
                    }

                    // Extra for matl_change:
                    else if (line == "[matl_change]")
                    {
                        line = in.readLine();
                        while (line.at(line.count() - 1) == ' ')
                            line.remove(line.length() - 1, 1);

                        QString path = QFileInfo(QFileInfo(object).dir().absolutePath() + "/texture/" + line).absoluteFilePath().remove(0, cutCount);
                        if (!checkTexture(QDir(QFileInfo(object).dir()).absolutePath() + "/texture/" + line, line))
                        {
                            qWarning().noquote() << "Error in object '" + current + "':" << "[matl_change] texture '" + line + "' could not be found!";
                            stuffobj.missing.textures << path;
                            qDebug() << "\n----------------------------------------------------------------------------------------------";
                        }
                        else
                            stuffobj.existing.textures << path;

                        in.readLine();
                        line = in.readLine();

                        // Check if varname exists
                        if (!variables.contains(line.toUpper()) && !preDefiniedLocalVariables.contains(line.toUpper()))
                        {
                            qWarning().noquote() << "Error in object '" + current + "':" << "[matl_change] variable '" + line + "' could not be found!";
                            qDebug().noquote() << "In List:" << variables << "(+ local pre-defined variables)";
                            stuffobj.missing.sceneryobjects << current;
                        }
                        else
                            stuffobj.existing.sceneryobjects << current;
                    }

                    // passengercabin:
                    else if (line == "[passengercabin]")
                    {
                        line = in.readLine();
                        while (line.at(line.count() - 1) == ' ')
                            line.remove(line.length() - 1, 1);

                        QString fullPath = QDir(QFileInfo(object).dir()).absolutePath() + "/" + line;
                        if (!QFile(fullPath).exists())
                        {
                            qWarning().noquote() << "Error in object '" + current + "':" << "Passengercabin '" + line + "' could not be found!";
                            stuffobj.missing.sceneryobjects << current;
                        }
                    }

                    // matl_freetex:
                    else if (line == "[matl_freetex]")
                    {
                        line = in.readLine();
                        while (line.at(line.count() - 1) == ' ')
                            line.remove(line.length() - 1, 1);

                        QString path = QFileInfo(QFileInfo(object).dir().absolutePath() + "/texture/" + line).absoluteFilePath().remove(0, cutCount);
                        if (!checkTexture(QDir(QFileInfo(object).dir()).absolutePath() + "/texture/" + line, line))
                        {
                            qWarning().noquote() << "Error in object '" + current + "':" << "[matl_freetex] Texture '" + line + "' could not be found!";
                            stuffobj.missing.textures << path;
                            qDebug() << "\n----------------------------------------------------------------------------------------------";
                        }
                        else
                            stuffobj.existing.textures << path;

                        line = in.readLine();

                        // Check if varname exists
                        if (!stringvariables.contains(line.toUpper()))
                        {
                            qWarning().noquote() << "Error in object '" + current + "':" << "[matl_freetex] variable '" + line + "' could not be found!";
                            qDebug().noquote() << "In List:" << stringvariables;
                            stuffobj.missing.sceneryobjects << current;
                        }
                    }
                }
                object.close();
            }
        }
    }

    /// verify a list of splines
    void verifySplines(QStringList &splines)
    {
        cutCount = set.read("main", "mainDir").toString().count() + 1;

        progressName = QObject::tr("Checking splines...");
        maxProgress = splines.count();
        int i = 0;

        foreach (QString current, splines)
        {
            i++;
            currentProgress = i;
            qApp->processEvents();

//            if (current == "Splines\\ADDON_Bad_Huegelsdorf\\Ueberland\\str_land_2spur_6m.sli")
//                qDebug().noquote() << "Here is " << current << "!";

            QFile spline(set.read("main", "mainDir").toString() + "/" + current);
            spline.open(QFile::ReadOnly | QFile::Text);

            if (spline.exists())
            {
                QTextStream in(&spline);
                in.setEncoding(QStringConverter::System);
                QString line;
                unsigned int profileCounter = 0;
                int lineCount = 0;

                while (!in.atEnd())
                {
                    lineCount++;
                    //qDebug().noquote() << "Line" << lineCount;
                    line = in.readLine();

                    // texure check:
                    if (line == "[texture]")
                    {
                        line = in.readLine();

                        if (line == "")
                        {
                            qWarning().noquote() << "Error in spline '" + current + "':" << "texture path is missing!";
                            stuffobj.missing.splines << current;
                            continue;
                        }

                        while (line.at(line.count() - 1) == ' ')
                            line.remove(line.length() - 1, 1);

                        QString fullPath = QDir(QFileInfo(spline).dir()).absolutePath() + "/" + line;
                        QString texPath = QFileInfo(QFileInfo(spline).dir().absolutePath() + "/texture/" + line).absoluteFilePath().remove(0, cutCount);
                        if (!checkTexture(QDir(QFileInfo(spline).dir()).absolutePath() + "/texture/" + line, line))
                        {
                            qWarning().noquote() << "Error in spline '" + current + "':" << "texture '" + line + "' could not be found!";
                            qDebug().noquote() << "ABSOLUTE PATH:" << QFileInfo(QFileInfo(spline).dir().absolutePath() + "/texture/" + line).absoluteFilePath();
                            // old: texPath
                            stuffobj.missing.textures << fullPath;
                            stuffobj.missing.splines << current;
                            qDebug() << "\n----------------------------------------------------------------------------------------------";
                        }
                        else
                        {
                            stuffobj.existing.textures << texPath;
                            stuffobj.existing.splines << current;
                        }
                    }

                    else if (line == "[profile]")
                        profileCounter++;
                }
                spline.close();

                if (profileCounter == 0)
                {
                    qWarning().noquote() << "Error in spline '" + current + "':" << "Spline has no profiles!";
                    stuffobj.missing.splines << current;
                }
            }
        }
    }

    /// Checks if a texture exists
    bool checkTexture(QString fullPath, QString relPath)
    {
        QString tempPath = fullPath;
        QString newPath = tempPath.remove(fullPath.length() - 3, 3) + "dds";
        QFile sharedTexture(set.read("main", "mainDir").toString() + "/Texture/" + relPath);
        QFile mainDirTexture(set.read("main", "mainDir").toString() + "/" + relPath);

        if (!QFile(fullPath).exists())                  // if the 'real' texture exists
            if (!sharedTexture.exists())                // make shared texture check
                if (!mainDirTexture.exists())           // make mainDir texture check
                    if (!QFile(newPath).exists())       // if the dds format of the texture exists
                    {
                        qDebug().noquote() << "Texture doesn't exist:";
                        qDebug().noquote() << "Full:" << fullPath;
                        qDebug().noquote() << "Rel:" << relPath;
                        qDebug().noquote() << "Shared:" << set.read("main", "mainDir").toString() + "/Texture/" + relPath;
                        qDebug().noquote() << "mainDirTex:" << set.read("main", "mainDir").toString() + "/" + relPath;
                        return false;
                    }

        return true;
    }

    /// Sets the map path
    void setMapPath(QString path)
    {
        mapPath = path;
    }

    /// Resturns the map path
    QString getMapPath()
    {
        return mapPath;
    }

    /// Returns results from global.cfg
    QString readGlobal(QString param, QWidget *parent = 0, int readLine = 1)
    {
        cutCount = set.read("main", "mainDir").toString().count() + 1;

        param = "[" + param + "]";

        QFile global(mapPath);
        if (!global.open(QFile::ReadOnly | QFile::Text))
        {
            msg.fileOpenErrorCloseOMSI(parent, mapPath);
            qDebug().noquote() << "Full path: '" + QFileInfo(global).absoluteFilePath() + "'";
            return "ERR";
        }

        QTextStream in(&global);
        in.setEncoding(QStringConverter::System);
        QString line = "";

        // Reading global.cfg
        while(!(in.atEnd()))
        {
            line = in.readLine();
            if (line == param)
            {
                for (int i = 0; i < readLine; i++)
                    line = in.readLine();
                global.close();
                return line;
            }
        }
        global.close();

        qDebug().noquote() << "Could not find param '" + param + "'!";
        return "?";
    }

    /// Gets tiles from a map
    void getTiles(QWidget *parent = 0)
    {
        cutCount = set.read("main", "mainDir").toString().count() + 1;

        QFile global(mapPath);
        if (!global.open(QFile::ReadOnly | QFile::Text))
        {
            msg.fileOpenErrorCloseOMSI(parent, mapPath);
            return;
        }

        QString line;
        QTextStream in(&global);
        in.setEncoding(QStringConverter::System);

        QString halfTilePath = mapPath;
        halfTilePath = halfTilePath.remove(QRegularExpression("global.cfg"));
        QStringList tiles;

        // normal tiles:
        while (!in.atEnd())
        {
            line = in.readLine();

            if (line == "[map]")
            {
                in.readLine();
                in.readLine();
                tiles << halfTilePath + in.readLine();
            }
        }

        // chronologies:
        const QString directory(QFileInfo(mapPath).absolutePath() + "/Chrono");
        QDirIterator chronoDirIterator(directory, QStringList("*.map"), QDir::Files, QDirIterator::Subdirectories);

        QStringList chronoTiles;
        while (chronoDirIterator.hasNext())
        {
            QString chronoTile = QFileInfo(chronoDirIterator.next()).absoluteFilePath();
            if (tiles.contains(halfTilePath + QFileInfo(chronoTile).fileName()))
                chronoTiles << chronoTile;
        }

        tiles.append(chronoTiles);
        tiles.removeDuplicates();

        foreach (QString current, tiles)
        {
            QFile tile(current);
            QFileInfo tileInfo(tile);

            if (tile.exists())
                stuffobj.existing.tiles << tileInfo.absoluteFilePath().remove(0, getMapPath().remove("global.cfg").length());
            else
            {
                stuffobj.missing.tiles << tileInfo.fileName();
                qWarning().noquote() << "Tile '" + tileInfo.fileName() + "' is missing!";
                qDebug().noquote() << "Full path: '" + tileInfo.absoluteFilePath() + "'";
            }
        }

        global.close();
    }

    /// Gets items from a map
    void getItems(QStringList &tiles)
    {
        cutCount = set.read("main", "mainDir").toString().count() + 1;

        progressName = QObject::tr("Get sceneryobjects and splines...");
        maxProgress = tiles.count();
        int i = 0;

        QString mainDir = set.read("main", "mainDir").toString();

        foreach (QString path, tiles)
        {
            qDebug().noquote() << QString("Tile: %1").arg(path);

            i++;
            currentProgress = i;
            qApp->processEvents();

            path = getMapPath().remove("global.cfg") + path;

            QFile tile(path);
            tile.open(QFile::ReadOnly | QFile::Text);

            QTextStream in(&tile);
            in.setEncoding(QStringConverter::System);
            QString line;

            while (!in.atEnd())
            {
                line = in.readLine();

                if (line == "[object]" || line == "[splineAttachement]" || line == "[attachObj]" || line == "[splineAttachement_repeater]")
                {
                    if (line == "[splineAttachement_repeater]")
                    {
                        in.readLine();
                        in.readLine();
                    }

                    in.readLine();
                    line = in.readLine();

                    QFile object(mainDir + "/" + line);
                    QString fullPath = QString(QFileInfo(object).absoluteFilePath()).remove(0, cutCount);

                    if (!object.exists())
                    {
                        qWarning().noquote() << "Sceneryobject '" + QFileInfo(object).absoluteFilePath() + "' is missing!";
                        stuffobj.missing.sceneryobjects << fullPath;
                    }
                    else
                        stuffobj.existing.sceneryobjects << fullPath;
                }
                else if (line == "[spline]" || line == "[spline_h]")
                {
                    in.readLine();
                    line = in.readLine();

                    QFile spline(mainDir + "/" + line);
                    QString fullPath = QString(QFileInfo(spline).absoluteFilePath()).remove(0, cutCount);

                    if (!spline.exists())
                    {
                        qWarning().noquote() << "Spline '" + QFileInfo(spline).absoluteFilePath() + "' is missing!";
                        stuffobj.missing.splines << fullPath;
                    }
                    else
                        stuffobj.existing.splines << fullPath;
                }
            }
        }

        // parklists
        QStringList parklists;
        parklists << getMapPath().remove("global.cfg") + "parklist_p.txt";

        for (int i = 1; i > 100; i++)
        {
            QString additionalParklist = getMapPath().remove("global.cfg") + "parklist_p_" + QString::number(i) + ".txt";
            if (QFile(additionalParklist).exists())
                parklists << additionalParklist;
        }

        // chronologies - normal:
        const QString directory(QFileInfo(mapPath).absolutePath() + "/Chrono");
        QDirIterator chronoDirIterator(directory, QStringList("parklist_p.txt"), QDir::Files, QDirIterator::Subdirectories);

        QStringList chronoParklists;
        while (chronoDirIterator.hasNext())
            chronoParklists << QFileInfo(chronoDirIterator.next()).absoluteFilePath();

        // chronologies - extra parklists:
        QDirIterator chronoDirIterator2(directory, QStringList("parklist_p_*.txt"), QDir::Files, QDirIterator::Subdirectories);

        while (chronoDirIterator2.hasNext())
            chronoParklists << QFileInfo(chronoDirIterator2.next()).absoluteFilePath();

        parklists << chronoParklists;

        foreach (QString current, parklists)
        {
            QFile parklist(current);

            QTextStream in(&parklist);
            in.setEncoding(QStringConverter::System);
            QString line;
            int lineCounter = 0;

            while (!in.atEnd())
            {
                line = in.readLine();
                lineCounter++;

                if (line == "")
                {
                    qWarning().noquote() << "Error in parklist '" + current.remove(getMapPath().remove("global.cfg")) + "': Line " + QString::number(lineCounter) + " is empty!";
                    stuffobj.missing.sceneryobjects << QObject::tr("[Empty line in parklist %1]").arg(current.remove(getMapPath().remove("global.cfg")));

                    continue;
                }

                QFile firstVehicle(mainDir + "/" + line);
                QString fullPath = QString(QFileInfo(firstVehicle).absoluteFilePath()).remove(0, cutCount);

                if (!firstVehicle.exists())
                {
                    qWarning().noquote() << "Sceneryobject '" + QFileInfo(firstVehicle).absoluteFilePath() + "' is missing!";
                    stuffobj.missing.sceneryobjects << fullPath;
                }
                else
                    stuffobj.existing.sceneryobjects << fullPath;

                firstVehicle.close();
            }
        }
    }

    /// Get vehicles of a map
    void getVehicles(QWidget *parent = 0)
    {
        cutCount = set.read("main", "mainDir").toString().count() + 1;

        QStringList aiLists;
        aiLists << getMapPath().remove("global.cfg") + "ailists.cfg";

        // chronologies:
        const QString directory(QFileInfo(mapPath).absolutePath() + "/Chrono");
        QDirIterator chronoDirIterator(directory, QStringList("ailists_#upd.cfg"), QDir::Files, QDirIterator::Subdirectories);

        QStringList chronoAiLists;
        while (chronoDirIterator.hasNext())
            chronoAiLists << QFileInfo(chronoDirIterator.next()).absoluteFilePath();

        aiLists << chronoAiLists;

        foreach (QString aiList, aiLists)
        {
            QFile aiListFile(aiList);
            if (!aiListFile.open(QFile::ReadOnly | QFile::Text))
            {
                msg.fileOpenErrorCloseOMSI(parent, "ailists.cfg");
                return;
            }

            QTextStream in(&aiListFile);
            in.setEncoding(QStringConverter::System);

            QString line;
            int lineCounter = 0;

            while (!in.atEnd())
            {
                line = in.readLine();
                lineCounter++;

                if (line == "[aigroup_2]")
                {
                    in.readLine();
                    lineCounter++;

                    in.readLine();
                    lineCounter++;

                    while (line != "[end]")
                    {
                        line = in.readLine();
                        lineCounter++;

                        if (line == "[end]")
                            break;

                        if (line == "")
                        {
                            qWarning().noquote() << "Error in AI list: Line " + QString::number(lineCounter) + " is empty!";
                            stuffobj.missing.vehicles << QObject::tr("[Empty line]");
                            continue;
                        }

                        // Cut all numbers, tabs and spaces
                        while (line.at(line.count() - 1).isNumber() || (line.at(line.count() - 1) == '\x9') || (line.at(line.count() - 1) == ' '))
                            line.remove(line.count() - 1, 1);

                        QFile vehicle(set.read("main", "mainDir").toString() + "/" + line);

                        if (!vehicle.exists())
                        {
                            qWarning().noquote() << "Vehicle '" + QFileInfo(vehicle).absoluteFilePath() + "' is missing!";
                            stuffobj.missing.vehicles << QString(QFileInfo(vehicle).absoluteFilePath()).remove(0, cutCount);
                        }
                        else
                            stuffobj.existing.vehicles << QString(QFileInfo(vehicle).absoluteFilePath()).remove(0, cutCount);
                    }
                }
                else if (line == "[aigroup_depot_typgroup_2]")
                {
                    line = in.readLine();

                    if (line == "")
                    {
                        qWarning().noquote() << "Error in AI list: Line " + QString::number(lineCounter) + " is empty!";
                        stuffobj.missing.vehicles << QObject::tr("[Empty line]");
                        continue;
                    }

                    QFile vehicle(set.read("main", "mainDir").toString() + "/" + line);

                    if (!vehicle.exists())
                    {
                        qWarning().noquote() << "Vehicle '" + QFileInfo(vehicle).absoluteFilePath() + "' is missing!";
                        stuffobj.missing.vehicles << QString(QFileInfo(vehicle).absoluteFilePath()).remove(0, cutCount);
                    }
                    else
                        stuffobj.existing.vehicles << QString(QFileInfo(vehicle).absoluteFilePath()).remove(0, cutCount);
                }
            }
            aiListFile.close();
        }
    }

    /// Get humans of a map
    void getHumans(QWidget *parent = 0)
    {
        cutCount = set.read("main", "mainDir").toString().count() + 1;

        for (int i = 0; i < 2; i++)
        {
            QString filePath;

            if (i == 0)
                filePath = getMapPath().remove("global.cfg") + "humans.txt";
            else
                filePath = getMapPath().remove("global.cfg") + "drivers.txt";

            QFile humans(filePath);
            if (!humans.open(QFile::ReadOnly | QFile::Text))
            {
                msg.fileOpenErrorCloseOMSI(parent, QFileInfo(humans).absoluteFilePath());
                return;
            }

            QTextStream in(&humans);
            in.setEncoding(QStringConverter::System);

            QString line;
            int lineCounter = 0;

            while (!in.atEnd())
            {
                line = in.readLine();
                lineCounter++;

                if (line == "")
                {
                    qWarning().noquote() << "Error in human file: Line " + QString::number(lineCounter) + " is empty!";
                    stuffobj.missing.humans << QObject::tr("[Empty line]");
                    continue;
                }

                QFile human(set.read("main", "mainDir").toString() + "/" + line);

                if (!human.exists())
                {
                    qWarning().noquote() << "Human '" + QFileInfo(human).absoluteFilePath() + "' is missing!";
                    stuffobj.missing.humans << QString(QFileInfo(human).absoluteFilePath()).remove(0, cutCount);
                }
                else
                    stuffobj.existing.humans << QString(QFileInfo(human).absoluteFilePath()).remove(0, cutCount);
            }
            humans.close();
        }
    }

    /// Checks if the texture layers of a map exists
    void checkTextureLayers(QWidget *parent = 0)
    {
        QStringList globalTextures;

        QFile global(getMapPath());
        if (!global.open(QFile::ReadOnly | QFile::Text))
        {
            msg.fileOpenErrorCloseOMSI(parent, getMapPath());
            return;
        }

        QTextStream in(&global);
        in.setEncoding(QStringConverter::System);

        while (!in.atEnd())
        {
            QString line = in.readLine();

            if (line == "[groundtex]")
            {
                line = in.readLine();
                globalTextures << line;

                line = in.readLine();
                globalTextures << line;
            }
        }

        global.close();

        globalTextures.removeDuplicates();

        foreach (QString current, globalTextures)
        {
            QFile texture(set.read("main", "mainDir").toString() + "/" + current);
            if (!texture.exists())
            {
                qWarning().noquote() << "Texture '" + QFileInfo(texture).absoluteFilePath() + "' is missing!";
                stuffobj.missing.globalTextures << QString(QFileInfo(texture).absoluteFilePath()).remove(0, set.read("main", "mainDir").toString().length() + 1);
            }
            else
                stuffobj.existing.globalTextures << QString(QFileInfo(texture).absoluteFilePath()).remove(0, set.read("main", "mainDir").toString().length() + 1);
        }
    }

    /// Opens a font
    OTFontModel openFont(QString path, bool utf8encoding = false)
    {
        OTFontModel font;
        font.path = path;

        QFile file(font.path);

        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            qWarning().noquote() << "Font could not be opened: '" + QFileInfo(file).absoluteFilePath() + "'";
            font.error = true;
            return font;
        }

        QTextStream in(&file);
        in.setEncoding(QStringConverter::Utf16);

//        if (utf8encoding)
//        {
//            qDebug() << "Set encoding to UTF-8";
//            in.setEncoding(QStringConverter::Utf8);
//        }

        QString line;
        int fontCounter = 0;

        try
        {
            while (!in.atEnd())
            {
                // Reading process: get line ...
                if (line == "[newfont]")
                {
                    fontCounter++;
                    // ... and save it to model
                    font.name = in.readLine();
                    font.colorTexture = in.readLine();
                    font.alphaTexture = in.readLine();
                    QString maxHeightOfChars = in.readLine();
                    font.maxHeightOfChars = (maxHeightOfChars == "") ? -1 : maxHeightOfChars.toInt();
                    QString distanceBetweenChars = in.readLine();
                    font.distanceBetweenChars = (distanceBetweenChars == "") ? -1 : distanceBetweenChars.toInt();
                }

                else if (line == "[char]")
                {
                    OTCharacterModel character;

                    character.character = in.readLine();

                    QString leftPixel = in.readLine();
                    character.leftPixel = (leftPixel == "") ? -1 : leftPixel.toInt();

                    QString rightPixel = in.readLine();
                    character.rightPixel = (rightPixel == "") ? -1 : rightPixel.toInt();

                    QString highestPixelInFontRow = in.readLine();
                    character.highestPixelInFontRow = (highestPixelInFontRow == "") ? -1 : highestPixelInFontRow.toInt();

                    line = in.readLine();

                    if (line == "[char]")
                        continue;
                    else if (line.contains("//"))
                        character.comment = line.remove(0, 3);

                    font.charList.append(character);
                }

                // Attention: This code needs to be here. Else there will be some problems with a [char] directly below a previous char entry.
                line = in.readLine();
            }
        }
        catch (...)
        {
            font.error = true;
            qCritical().noquote() << "Font could not be read: '" + QFileInfo(file).absoluteFilePath() + "'";
            return font;
        }

        if (fontCounter > 1)
            font.moreThanOneFont = true;

        file.close();

        return font;
    }

    /// Saves a font
    bool saveFont(OTFontModel font, bool utf8encoding = false)
    {
        QFile file(font.path);

        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            qWarning().noquote() << "Font could not be saved: '" + QFileInfo(file).absoluteFilePath() + "'";
            return false;
        }

        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf16);

//        if (utf8encoding)
//        {
//            qDebug() << "Set encoding to UTF-8";
//            out.setEncoding(QStringConverter::Utf8);
//        }

        out << fop.writeFileHeader();

        QString extraHashs = "##";

        // set in fontnameLength x "#" to cover the name completely
        for (int i = 0; i < font.name.length(); i++)
            extraHashs += "#";

        // Header No. 2
        out << "####################" << extraHashs << "\n";
        out << "Font name:\t\t\t" << font.name << "\n";
        out << "Total characters:\t" << font.charList.count() << "\n";
        out << "Author:\t\t\t\t" << set.read("main", "author").toString() << "\n";
        out << "####################" << extraHashs << "\n";
        out << "\n";

        out << "[newfont]" << "\n";
        out << font.name << "\n";
        out << font.colorTexture << "\n";
        out << font.alphaTexture << "\n";

        // variable = (condition) ? expressionTrue : expressionFalse;

        QString maxHeightOfChars = (font.maxHeightOfChars == -1) ? "" : QString::number(font.maxHeightOfChars);
        out << maxHeightOfChars << "\n";
        QString distanceBetweenChars = (font.distanceBetweenChars == -1) ? "" : QString::number(font.distanceBetweenChars);
        out << distanceBetweenChars << "\n";

        out << "\n";

        foreach (OTCharacterModel current, font.charList)
        {
            // Writing process
            out << "[char]" << "\n";
            out << current.character << "\n";

            QString leftPixel = (current.leftPixel == -1) ? "" : QString::number(current.leftPixel);
            out << leftPixel << "\n";

            QString rightPixel = (current.rightPixel == -1) ? "" : QString::number(current.rightPixel);
            out << rightPixel << "\n";

            QString highestPixelInFontRow = (current.highestPixelInFontRow == -1) ? "" : QString::number(current.highestPixelInFontRow);
            out << highestPixelInFontRow << "\n";

            // Output: [Tab]// [Comment]
            if (current.comment != "")
                out << "\t// " << current.comment << "\n";

            out << "\n";
        }

        file.flush();
        file.close();

        return true;
    }

private:
    OTSettings set;
    OTFileOperations fop;
    QString mapPath;
    OTMessage msg;
    int cutCount = set.read("main", "mainDir").toString().count() + 1;
};

#endif // OTOMSIFILEHANDLER_H

