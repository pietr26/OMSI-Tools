#ifndef OTOMSIFILEHANDLER_H
#define OTOMSIFILEHANDLER_H
#include "OTGlobal.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include "OTOmsiFileHandler_models.h"

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

    /// Returns a QStringList with all variables - needs an absolute path
    QStringList readVarlist(QString path)
    {
        cutCount = set.read("main", "mainDir").toString().length() + 1;

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
        const QString mainDir = set.read("main", "mainDir").toString();

        cutCount = mainDir.length() + 1;

        progressName = QObject::tr("Checking sceneryobjects...");
        maxProgress = objects.length();
        int i = 0;

        foreach (QString current, objects)
        {
            i++;
            qDebug().noquote().nospace() << "Object " << i << ": " << current;
            currentProgress = i;
            qApp->processEvents();

//            if (current.contains("Komin_1.sco"))
//                qDebug().noquote() << "Here is " << current << "!";

            QFile object(mainDir + "/" + current);
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

                            while ((varlist.endsWith(' ')))
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

                            while ((stringvarlist.endsWith(' ')))
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

                        while ((line.endsWith(' ')))
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

                            while ((scriptPath.endsWith(' ')))
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

                            while ((constfilePath.endsWith(' ')))
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

                        while ((line.endsWith(' ')))
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

                        while ((line.endsWith(' ')))
                            line.remove(line.length() - 1, 1);

                        if (!textTextures.contains(line))
                        {
                            QString path = QFileInfo(QFileInfo(object).dir().absolutePath() + "/texture/" + line).absoluteFilePath().remove(0, cutCount);

                            if (!checkTexture(QDir(QFileInfo(object).dir()).absolutePath() + "/texture/" + line, line))
                            {
                                qWarning().noquote() << "Error in object '" + current + "':" << "Texture '" + line + "' could not be found!";
                                // old: path
                                stuffobj.missing.textures << QDir(QFileInfo(object).dir()).absolutePath() + "/texture/" + line;
                                qDebug().noquote() << "Abs:" << QFileInfo(QFileInfo(object).dir().absolutePath() + "/texture/" + line).absoluteFilePath();
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

                        while ((line.endsWith(' ')))
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

                        while ((line.endsWith(' ')))
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

                        while ((line.endsWith(' ')))
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
        const QString mainDir = set.read("main", "mainDir").toString();

        cutCount = mainDir.length() + 1;

        /*progressName = QObject::tr("Checking splines...");
        maxProgress = splines.length();*/

        foreach (QString current, splines)
        {
            /*currentProgress = i;*/
            qApp->processEvents();

            //            if (current == "Splines\\ADDON_Bad_Huegelsdorf\\Ueberland\\str_land_2spur_6m.sli")
            //                qDebug().noquote() << "Here is " << current << "!";

            QFile spline(mainDir + "/" + current);
            spline.open(QFile::ReadOnly | QFile::Text);

            if (spline.exists())
            {
                QTextStream in(&spline);
                in.setEncoding(QStringConverter::System);
                QString line;
                unsigned int profileCounter = 0;

                while (!in.atEnd())
                {
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

                        while (line.at(line.length() - 1) == ' ')
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
        const QString mainDir = set.read("main", "mainDir").toString();

        if (fullPath.endsWith("/")) return false; // means in context that no explicit file name is given for this path

        QString tempPath = fullPath;
        QString newPath = tempPath.remove(fullPath.length() - 3, 3) + "dds";
        QFile sharedTexture(mainDir + "/Texture/" + relPath);
        QFile mainDirTexture(mainDir + "/" + relPath);

        if (!QFile(fullPath).exists())                  // if the 'real' texture exists
            if (!sharedTexture.exists())                // make shared texture check
                if (!mainDirTexture.exists())           // make mainDir texture check
                    if (!QFile(newPath).exists())       // if the dds format of the texture exists
                    {
                        qDebug().noquote() << "Texture doesn't exist:";
                        qDebug().noquote() << "Full:" << fullPath;
                        qDebug().noquote() << "Rel:" << relPath;
                        qDebug().noquote() << "Shared:" << mainDir + "/Texture/" + relPath;
                        qDebug().noquote() << "mainDirTex:" << mainDir + "/" + relPath;
                        return false;
                    }

        return true;
    }

    /// Sets the map path
    void setMapPath(QString path, bool clearStuffobj = false)
    {
        mapPath = path;

        if (clearStuffobj) stuffobj.clear();
    }

    /// Resturns the map path
    QString getMapPath()
    {
        return mapPath;
    }

    /// Returns a list of all maps - first: name; second: full path
    QList<QPair<QString, QString>> listMaps()
    {
        QList<QPair<QString, QString>> returnList;
        const QString mapFolderPath = set.read("main", "mainDir").toString() + "/maps";

        QDirIterator mapFolder(mapFolderPath, QDir::Dirs | QDir::NoDotAndDotDot);
        while (mapFolder.hasNext())
        {
            QPair<QString, QString> pair;
            pair.second = mapFolder.next() + "/global.cfg";
            if (!QFile(pair.second).exists()) continue;
            pair.first = readConfig("[friendlyname]", pair.second);
            if (pair.first != "ERR")
                returnList.append(pair);
        }

        return returnList;
    }

    /// Returns a list of all moneypacks - first: name; second: full path
    QList<QPair<QString, QString>> listMoney()
    {
        QList<QPair<QString, QString>> returnList;
        QString mainDir = set.read("main", "mainDir").toString();

        QDirIterator moneyFolder(mainDir + "/Money", { "*.cfg" }, QDir::Files, QDirIterator::Subdirectories);
        while (moneyFolder.hasNext())
        {
            QString next = moneyFolder.next();

            QPair<QString, QString> pair;
            pair.second = next;
            pair.first = next.remove(mainDir + "/Money/") + " (" + readConfig("[currency]", pair.second) + ")";
            if (pair.first != "ERR")
                returnList.append(pair);
        }

        return returnList;
    }

    /// Returns a list of all ticketpacks - first: name; second: full path
    QList<QPair<QString, QString>> listTicketpacks()
    {
        QList<QPair<QString, QString>> returnList;
        QString mainDir = set.read("main", "mainDir").toString();

        QDirIterator ticketpackFolder(mainDir + "/TicketPacks", { "*.otp" }, QDir::Files, QDirIterator::Subdirectories);
        while (ticketpackFolder.hasNext())
        {
            QString next = ticketpackFolder.next();

            QPair<QString, QString> pair;
            pair.second = next;
            pair.first = next.remove(mainDir + "/TicketPacks/");

            returnList.append(pair);
        }

        return returnList;
    }

    /// Returns results from any configuration file
    QString readConfig(QString fullLineBeforeContent, QString absolutePath, int readLine = 1)
    {
        if (absolutePath.isEmpty()) return "ERR";

        cutCount = set.read("main", "mainDir").toString().length() + 1;

        QFile file(absolutePath);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug().noquote() << "Cannot open file in readConfig(): Full path: '" + QFileInfo(file).absoluteFilePath() + "'";
            return "ERR";
        }

        QTextStream in(&file);
        in.setEncoding(QStringConverter::System);
        QString line = "";

        while(!in.atEnd())
        {
            line = in.readLine();
            if (line == fullLineBeforeContent)
            {
                for (int i = 0; i < readLine; i++) line = in.readLine();
                file.close();
                return line;
            }
        }
        file.close();

        qDebug().noquote() << "Could not find line after '" + fullLineBeforeContent + "' in file '" + absolutePath + "'!";
        return "ERR";
    }

    /// Gets tiles from a map
    void getTiles(QWidget *parent = 0)
    {
        cutCount = set.read("main", "mainDir").toString().length() + 1;

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
        halfTilePath = halfTilePath.remove("global.cfg");
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
    void getItems(QStringList &tiles, bool checkMissing = true, bool includeParklists = true)
    {
        const QString mainDir = set.read("main", "mainDir").toString();

        cutCount = mainDir.length() + 1;

        // simplier description.
        progressName = QObject::tr("Read tiles (%1 / %2)").arg("0", "0");

        QStringList a, b;
        if (tiles.length() == 1)
            a = tiles;
        else
        {
            a = tiles.mid(0, (tiles.length() / 2));
            b = tiles.mid(tiles.length() / 2, tiles.length() - 1);
        }

        maxProgress = tiles.length();
        currentProgress = 0;

        // List a
        QFuture<void> aFuture;
        QFutureWatcher<void> *aFutureWatcher = new QFutureWatcher<void>();
        QEventLoop *aLoop = new QEventLoop();

        QElapsedTimer elTimer;
        elTimer.start();
        aFuture = QtConcurrent::run([=]() { getItemsFromTileList(a, checkMissing, "a"); });
        QObject::connect(aFutureWatcher, &QFutureWatcher<void>::finished, [=]()
        {
            qDebug().noquote() << QString("getItemThread a finished (%1s)").arg(elTimer.elapsed() / 1000);
            aLoop->quit();
        });

        aFutureWatcher->setFuture(aFuture);

        // List b
        QFuture<void> bFuture;
        QFutureWatcher<void> *bFutureWatcher = new QFutureWatcher<void>();
        QEventLoop *bLoop = new QEventLoop();

        if (tiles.length() > 1)
        {
            bFuture = QtConcurrent::run([=]() { getItemsFromTileList(b, checkMissing, "b"); });
            QObject::connect(bFutureWatcher, &QFutureWatcher<void>::finished, [=]()
            {
                qDebug().noquote() << QString("getItemThread b finished (%1s)").arg(elTimer.elapsed() / 1000);
                bLoop->quit();
            });

            bFutureWatcher->setFuture(bFuture);
        }

        // ---

        if (includeParklists)
        {
            // PARKLISTS
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

                if (!parklist.open(QFile::ReadOnly | QFile::Text))
                {
                    qWarning().noquote() << "Cannot read Parklist:" << current;
                    continue;
                }

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
                        if (checkMissing)
                        {
                            qWarning().noquote() << "Error in parklist '" + current.remove(getMapPath().remove("global.cfg")) + "': Line " + QString::number(lineCounter) + " is empty!";
                            stuffobj.missing.sceneryobjects << QObject::tr("[Empty line in parklist %1]").arg(current.remove(getMapPath().remove("global.cfg"))); // TODO
                        }

                        continue;
                    }

                    QFile firstVehicle(mainDir + "/" + line);
                    QString fullPath = QString(QFileInfo(firstVehicle).absoluteFilePath()).remove(0, cutCount);

                    if (!firstVehicle.exists())
                    {
                        if (checkMissing)
                        {
                            qWarning().noquote() << "Sceneryobject '" + QFileInfo(firstVehicle).absoluteFilePath() + "' is missing!";
                            stuffobj.missing.sceneryobjects << fullPath;
                        }
                    }
                    else
                        stuffobj.existing.sceneryobjects << fullPath;

                    firstVehicle.close();
                }
            }
        }

        aLoop->exec();
        if (!bFuture.isFinished()) // If the Future is already finished, bLoop->exec(); would wait infinite time for an quit command (which was already called)
            bLoop->exec();

        aFutureWatcher->deleteLater();
        aLoop->deleteLater();
        bFutureWatcher->deleteLater();
        bLoop->deleteLater();
    }

    /// Get vehicles of a map
    void getVehicles(QWidget *parent = 0)
    {
        const QString mainDir = set.read("main", "mainDir").toString();

        cutCount = mainDir.length() + 1;

        QStringList aiLists;
        aiLists << getMapPath().remove("global.cfg") + "ailists.cfg";

        // chronologies:
        const QString directory(QFileInfo(mapPath).absolutePath() + "/Chrono");
        QDirIterator chronoDirIterator(directory, QStringList("ailists_#upd.cfg"), QDir::Files, QDirIterator::Subdirectories);

        QStringList chronoAiLists;
        while (chronoDirIterator.hasNext())
            chronoAiLists << QFileInfo(chronoDirIterator.next()).absoluteFilePath();

        aiLists << chronoAiLists;

        QStringList trains;

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

                    while (!in.atEnd())
                    {
                        line = in.readLine();
                        lineCounter++;

                        if (line == "[end]")
                            break;

                        if (line == "")
                        {
                            qWarning().noquote() << "Error in AI list: Line " + QString::number(lineCounter) + " is empty!";
                            continue;
                        }

                        // Cut all numbers, tabs and spaces
                        while (line.at(line.length() - 1).isNumber() || (line.at(line.length() - 1) == '\x9') || (line.at(line.length() - 1) == ' '))
                            line.remove(line.length() - 1, 1);

                        QFile vehicle(mainDir + "/" + line);

                        if (!vehicle.exists())
                        {
                            qWarning().noquote() << "Vehicle '" + QFileInfo(vehicle).absoluteFilePath() + "' is missing!";
                            stuffobj.missing.vehicles << QString(QFileInfo(vehicle).absoluteFilePath()).remove(0, cutCount);
                        }
                        else
                        {
                            stuffobj.existing.vehicles << QString(QFileInfo(vehicle).absoluteFilePath()).remove(0, cutCount);
                            if (line.endsWith(".zug")) trains << QFileInfo(vehicle).absoluteFilePath();
                        }
                    }
                }
                else if (line == "[aigroup_depot_typgroup_2]")
                {
                    line = in.readLine();

                    if (line == "")
                    {
                        qWarning().noquote() << "Error in AI list: Line " + QString::number(lineCounter) + " is empty!";
                        continue;
                    }

                    QFile vehicle(mainDir + "/" + line);

                    if (!vehicle.exists())
                    {
                        qWarning().noquote() << "Vehicle '" + QFileInfo(vehicle).absoluteFilePath() + "' is missing!";
                        stuffobj.missing.vehicles << QString(QFileInfo(vehicle).absoluteFilePath()).remove(0, cutCount);
                    }
                    else
                    {
                        stuffobj.existing.vehicles << QString(QFileInfo(vehicle).absoluteFilePath()).remove(0, cutCount);
                        if (line.endsWith(".zug")) trains << QFileInfo(vehicle).absoluteFilePath();
                    }
                }
            }

            aiListFile.close();
        }

        trains.removeDuplicates();

        foreach (QString current, trains)
        {
            QFile trainFile(current);
            if (!trainFile.open(QFile::ReadOnly | QFile::Text))
            {
                msg.fileOpenErrorCloseOMSI(parent, current);
                return;
            }

            QTextStream in(&trainFile);
            in.setEncoding(QStringConverter::System);

            QString line;

            while (!in.atEnd())
            {
                line = in.readLine();

                QFile train(mainDir + "/" + line);

                if (!train.exists())
                {
                    qWarning().noquote() << "Vehicle (Train) '" + QFileInfo(train).absoluteFilePath() + "' is missing!";
                    stuffobj.missing.vehicles << QString(QFileInfo(train).absoluteFilePath()).remove(0, cutCount);
                }
                else
                    stuffobj.existing.vehicles << QString(QFileInfo(train).absoluteFilePath()).remove(0, cutCount);

                line = in.readLine(); // skip direction indicator
            }

            trainFile.close();
        }
    }

    /// Get humans of a map
    void getHumans(QWidget *parent = 0)
    {
        const QString mainDir = set.read("main", "mainDir").toString();

        cutCount = mainDir.length() + 1;

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
                    continue;
                }

                QFile human(mainDir + "/" + line);

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
        const QString mainDir = set.read("main", "mainDir").toString();

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
            QFile texture(mainDir + "/" + current);
            if (!texture.exists())
            {
                qWarning().noquote() << "Texture '" + QFileInfo(texture).absoluteFilePath() + "' is missing!";
                stuffobj.missing.globalTextures << QString(QFileInfo(texture).absoluteFilePath()).remove(0, mainDir.length() + 1);
            }
            else
                stuffobj.existing.globalTextures << QString(QFileInfo(texture).absoluteFilePath()).remove(0, mainDir.length() + 1);
        }
    }

    /// Opens a font
    OTFontModel openFont(QString path, QStringConverter::Encoding encoding)
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
        in.setEncoding(encoding);
        qDebug().noquote() << "Font encoding:" << encoding;

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
                    font.name = in.readLine().toUtf8();
                    font.colorTexture = in.readLine().toUtf8();
                    font.alphaTexture = in.readLine().toUtf8();
                    QString maxHeightOfChars = in.readLine().toUtf8();
                    font.maxHeightOfChars = (maxHeightOfChars == "") ? -1 : maxHeightOfChars.toInt();
                    QString distanceBetweenChars = in.readLine().toUtf8();
                    font.distanceBetweenChars = (distanceBetweenChars == "") ? -1 : distanceBetweenChars.toInt();
                }

                else if (line == "[char]")
                {
                    OTCharacterModel character;

                    character.character = in.readLine().toUtf8();

                    QString leftPixel = in.readLine().toUtf8();
                    character.leftPixel = (leftPixel == "") ? -1 : leftPixel.toInt();

                    QString rightPixel = in.readLine().toUtf8();
                    character.rightPixel = (rightPixel == "") ? -1 : rightPixel.toInt();

                    QString highestPixelInFontRow = in.readLine().toUtf8();
                    character.highestPixelInFontRow = (highestPixelInFontRow == "") ? -1 : highestPixelInFontRow.toInt();

                    line = in.readLine().toUtf8();

                    if (line == "[char]")
                        continue;
                    else if (line.contains("//"))
                        character.comment = line.remove(0, 3);

                    font.charList.append(character);
                }

                // Attention: This code needs to be here. Else there will be some problems with a [char] directly below a previous char entry.
                line = in.readLine().toUtf8();
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
    bool saveFont(OTFontModel font)
    {
        QFile file(font.path);

        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            qWarning().noquote() << "Font could not be saved: '" + QFileInfo(file).absoluteFilePath() + "'";
            return false;
        }

        QTextStream out(&file);

        out.setEncoding(QStringConverter::Latin1);

        out << fop.writeFileHeader();

        QString extraHashs = "##";

        // set in fontnameLength x "#" to cover the name completely
        for (int i = 0; i < font.name.length(); i++)
            extraHashs += "#";

        // Header No. 2
        out << "####################" << extraHashs << "\n";
        out << "Font name:\t\t\t" << font.name << "\n";
        out << "Total characters:\t" << font.charList.length() << "\n";
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
    int cutCount = set.read("main", "mainDir").toString().length() + 1;

    QMutex mutex;

    void getItemsFromTileList(QStringList tileList, bool checkMissing, QString thread)
    {
        const QString mainDir = set.read("main", "mainDir").toString();

        foreach (QString path, tileList)
        {
            qDebug().noquote().nospace() << "getItemThread " << thread << ": " << path;

            mutex.lock();
            currentProgress++;
            mutex.unlock();

            path = getMapPath().remove("global.cfg") + path;

            QFile tile(path);
            tile.open(QFile::ReadOnly | QFile::Text);

            QTextStream in(&tile);
            in.setEncoding(QStringConverter::System);
            QString line;

            while (!in.atEnd())
            {
                if (thread == "a") progressName = QObject::tr("Read tiles (%1 / %2)").arg(currentProgress).arg(maxProgress);
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
                        if (checkMissing)
                        {
                            qWarning().noquote() << "Sceneryobject '" + QFileInfo(object).absoluteFilePath() + "' is missing!";

                            mutex.lock();
                            stuffobj.missing.sceneryobjects << fullPath;
                            mutex.unlock();
                        }
                    }
                    else
                    {
                        mutex.lock();
                        stuffobj.existing.sceneryobjects << fullPath;
                        mutex.unlock();
                    }
                }
                else if (line == "[spline]" || line == "[spline_h]")
                {
                    in.readLine();
                    line = in.readLine();

                    QFile spline(mainDir + "/" + line);
                    QString fullPath = QString(QFileInfo(spline).absoluteFilePath()).remove(0, cutCount);

                    if (!spline.exists())
                    {
                        if (checkMissing)
                        {
                            qWarning().noquote() << "Spline '" + QFileInfo(spline).absoluteFilePath() + "' is missing!";

                            mutex.lock();
                            stuffobj.missing.splines << fullPath;
                            mutex.unlock();
                        }
                    }
                    else
                    {
                        mutex.lock();
                        stuffobj.existing.splines << fullPath;
                        mutex.unlock();
                    }
                }
            }
        }
    }
};

#endif // OTOMSIFILEHANDLER_H

