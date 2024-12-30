// TODO: Chrono Events
// TODO: Advanced verifying:
/*
 * SCO:
    - Mesh
    - Materials
    - Texturen
    - Varlists

 * SLI:
    - Texturen
*/
#include "OTMapScanner.h"

OTMapChecker::OTMapChecker(QObject *parent) :
    QThread(parent),
    _finish(false) {
}

void OTMapChecker::run() {
    _finish = false;
    _allSceneryobjects.clear();
    _allSplines.clear();
    _allHumans.clear();
    _allVehicles.clear();
    _missingSceneryobjects.clear();
    _missingSplines.clear();
    _missingHumans.clear();
    _missingVehicles.clear();

    while (true) {
        QMutexLocker locker(&_mutex);
        if (_queue.isEmpty()) {
            if (_finish) break;
            _dataAvailable.wait(&_mutex);
        }
        if (!_queue.isEmpty()) {
            QList<QPair<QString,QString>> files = _queue.dequeue();
            locker.unlock();

            for(QPair<QString,QString> pair : files) {
                QString file = pair.first;

                OTFileSource *source = findOrCreateSourceObject(file);
                source->addSource(pair.second);

                if(!QFile::exists(_omsiDir + "/" + file)) {
                    switch(source->fileType()) {
                        case OTFileSource::SceneryobjectFile: _missingSceneryobjects << file; break;
                        case OTFileSource::SplineFile:        _missingSplines        << file; break;
                        case OTFileSource::HumanFile:         _missingHumans         << file; break;
                        case OTFileSource::VehicleFile:       _missingVehicles       << file; break;
                        default: break;
                    }
                }
            }
        }
    }

    // process stuff    
    std::sort(_allSceneryobjects.begin(),     _allSceneryobjects.end(), [](OTFileSource a, OTFileSource b){return a.fileName() < b.fileName();});
    std::sort(_allSplines.begin(),            _allSplines.end(),        [](OTFileSource a, OTFileSource b){return a.fileName() < b.fileName();});
    std::sort(_allHumans.begin(),             _allHumans.end(),         [](OTFileSource a, OTFileSource b){return a.fileName() < b.fileName();});
    std::sort(_allVehicles.begin(),           _allVehicles.end(),       [](OTFileSource a, OTFileSource b){return a.fileName() < b.fileName();});

    std::sort(_missingSceneryobjects.begin(), _missingSceneryobjects.end());
    std::sort(_missingSplines.begin(),        _missingSplines.end());
    std::sort(_missingHumans.begin(),         _missingHumans.end());
    std::sort(_missingVehicles.begin(),       _missingVehicles.end());
}

QString OTMapChecker::omsiDir() const {
    return _omsiDir;
}

void OTMapChecker::setOmsiDir(const QString &str) {
    _omsiDir = str;
}

void OTMapChecker::addToQueue(const QList<QPair<QString, QString>> &list) {
    QMutexLocker locker(&_mutex);
    _queue.enqueue(list);
    _dataAvailable.wakeOne();
}

void OTMapChecker::addToQueue(const QStringList &list, const QString &source) {
    QList<QPair<QString, QString>> result;

    for(QString str : list)
        result << QPair<QString, QString>(str, source);
    addToQueue(result);
}

void OTMapChecker::setFinished() {
    QMutexLocker locker(&_mutex);
    _finish = true;
    _dataAvailable.wakeOne();
}

QList<OTFileSource> OTMapChecker::allSceneryobjects() const {
    return _allSceneryobjects;
}

QList<OTFileSource> OTMapChecker::allSplines() const {
    return _allSplines;
}

QList<OTFileSource> OTMapChecker::allHumans() const {
    return _allHumans;
}

QList<OTFileSource> OTMapChecker::allVehicles() const {
    return _allVehicles;
}

QStringList OTMapChecker::missingSceneryobjects() const {
    return _missingSceneryobjects;
}

QStringList OTMapChecker::missingSplines() const {
    return _missingSplines;
}

QStringList OTMapChecker::missingHumans() const {
    return _missingHumans;
}

QStringList OTMapChecker::missingVehicles() const {
    return _missingVehicles;
}

int OTMapChecker::allSceneryobjectsCount() const {
    return _allSceneryobjects.count();
}

int OTMapChecker::allSplinesCount() const {
    return _allSplines.count();
}

int OTMapChecker::allHumansCount() const {
    return _allHumans.count();
}

int OTMapChecker::allVehiclesCount() const {
    return _allVehicles.count();
}

int OTMapChecker::missingSceneryobjectsCount() const {
    return _missingSceneryobjects.count();
}

int OTMapChecker::missingSplinesCount() const {
    return _missingSplines.count();
}

int OTMapChecker::missingHumansCount() {
    return _missingHumans.count();
}

int OTMapChecker::missingVehiclesCount() const {
    return _missingVehicles.count();
}

OTFileSource *OTMapChecker::findOrCreateSourceObject(const QString &fileName) {
    if(fileName.endsWith(".sco")) {
        for(int i = 0; i < _allSceneryobjects.count(); i++)
            if(_allSceneryobjects[i].fileName() == fileName)
                return &_allSceneryobjects[i];

        _allSceneryobjects << OTFileSource(fileName, OTFileSource::SceneryobjectFile);
        return &_allSceneryobjects.last();
    } else if(fileName.endsWith(".sli")) {
        for(int i = 0; i < _allSplines.count(); i++)
            if(_allSplines[i].fileName() == fileName)
                return &_allSplines[i];

        _allSplines << OTFileSource(fileName, OTFileSource::SplineFile);
        return &_allSplines.last();
    } else if(fileName.endsWith(".hum")) {
        for(int i = 0; i < _allHumans.count(); i++)
            if(_allHumans[i].fileName() == fileName)
                return &_allHumans[i];

        _allHumans << OTFileSource(fileName, OTFileSource::HumanFile);
        return &_allHumans.last();
    } else if(fileName.endsWith(".ovh") || fileName.endsWith(".bus") || fileName.endsWith(".zug")) {
        for(int i = 0; i < _allVehicles.count(); i++)
            if(_allVehicles[i].fileName() == fileName)
                return &_allVehicles[i];

        _allVehicles << OTFileSource(fileName, OTFileSource::VehicleFile);
        return &_allVehicles.last();
    }

    qCritical() << "Couldn't handle source object: " << fileName;

    return nullptr;
}

// ------------------------------------------------------------------------

OTMapScanner::OTMapScanner(QObject *parent, OTMapChecker *checker) :
    QThread(parent),
    _checker(checker)
{}

void OTMapScanner::run() {
    _allTiles.clear();
    _missingTiles.clear();
    _allTextures.clear();
    _missingTextures.clear();

    scanGlobal();
    scanTextures();

    // read chrono events

    qInfo() << "reading chrono events";
    QStringList chronoTiles;
    QDir chronoDir(_mapDir + "/Chrono");
    if(chronoDir.exists()) {
        QStringList chronoList = chronoDir.entryList(QDir::NoDotAndDotDot|QDir::Dirs);
        for(QString currentChrono : chronoList) {
            QDir currentDir(_mapDir + "/Chrono/" + currentChrono);
            QStringList currentChronoList = currentDir.entryList(QDir::Files);
            if(!currentChronoList.contains("Chrono.cfg"))
                continue;

            for(QString currentFile : currentChronoList) {
                if(currentFile.endsWith(".map")) {
                    chronoTiles << "Chrono/" + currentChrono + "/" + currentFile;
                    OTFileSource *source = findOrCreateSourceObject(currentFile);
                    source->addSource("Chrono/" + currentChrono);
                }
            }
        }
    }

    QStringList combinedList;


    for(OTFileSource &source : _allTiles)
        combinedList << source.fileName();

    combinedList << chronoTiles;

    int tileCount = combinedList.count();
    emit initActionCount(tileCount);
    QString totalString = " /";

    qInfo() << "reading tiles";
    int i = 1;
    for(QString str : combinedList) {
        emit statusUpdate(i, tr("Read tile %1 of %2").arg(QString::number(i), QString::number(tileCount)));
        scanTile(str);
        i++;
    }

    scanParkLists();
    scanHumans();
    scanAiList();

    _checker->setFinished();
}

void OTMapScanner::setMapDir(const QString &str) {
    _mapDir = str;
}

void OTMapScanner::scanGlobal() {
    QFile f(_mapDir + "/global.cfg");

    qInfo() << "reading global.cfg";
    if(!f.exists()) {
        qWarning() << "global.cfg not found";
        return;
    }

    if(!f.open(QFile::ReadOnly)) {
        qWarning() << "Could not open global.cfg";
        return;
    }

    QTextStream s(&f);

    while(!s.atEnd()) {
        QString line = s.readLine();

        if(line == "[map]") {
            s.readLine();
            s.readLine();
            findOrCreateSourceObject(s.readLine(), false);
        } else if(line == "[groundtex]") {
            QString tex1 = s.readLine();
            QString tex2 = s.readLine();

            findOrCreateSourceObject(tex1, true);
            findOrCreateSourceObject(tex2, true);
        }
    }

    f.close();
}

void OTMapScanner::scanTextures() {
    QString omsiDir = _checker->omsiDir();
    for(OTFileSource &current : _allTextures) {
        if(!QFile::exists(omsiDir + "/" + current.fileName()) && !_missingTextures.contains(current.fileName()))
            _missingTextures << current.fileName();
    }
}

void OTMapScanner::scanParkLists() {
    int i = 0;

    while(true) {
        QString indexStr = i != 0 ? "_" + QString::number(i) : "";
        QString path = _mapDir + "/parklist_p" + indexStr + ".txt";
        QFile f(path);
        QString fileName = "parklist_p" + indexStr + ".txt";

        qInfo() << "reading " << fileName;
        if(!f.exists() && i != 0)
            break;
        else if(!f.exists()) {
            qWarning() << "parklist_p.txt not found!";
            break;
        }

        if(!f.open(QFile::ReadOnly)) {
            qWarning() << "Could not open " << fileName;
            break;
        }

        QTextStream s(&f);
        QStringList list;
        while(!s.atEnd()) {
            list << s.readLine();
        }
        f.close();
        _checker->addToQueue(list,fileName);
        i++;
    }
}

void OTMapScanner::scanHumans() {
    QFile f(_mapDir + "/humans.txt");
    qInfo() << "reading humans.txt";
    if(!f.exists()) {
        qWarning() << "humans.txt not found!";
        return;
    }

    if(!f.open(QFile::ReadOnly)) {
        qWarning() << "Could not open humans.txt";
        return;
    }

    QTextStream s(&f);
    QStringList list;
    while(!s.atEnd()) {
        list << s.readLine();
    }
    f.close();
    _checker->addToQueue(list, "humans.txt");
}

void OTMapScanner::scanAiList() {
    QString omsiDir = _checker->omsiDir();
    qInfo() << "reading ailists.cfg";
    QFile f(_mapDir + "/ailists.cfg");
    if(!f.exists()) {
        qWarning() << "ailists.cfg not found!";
        return;
    }

    if(!f.open(QFile::ReadOnly)) {
        qWarning() << "Could not open ailists.cfg";
        return;
    }

    QTextStream s(&f);
    QStringList list;
    while(!s.atEnd()) {
        QString line = s.readLine();

        if(line == "[aigroup_2]") {
            s.readLine();
            s.readLine();
            line = s.readLine();
            while(line != "[end]" && !s.atEnd()) {
                QString file = line.split("\t")[0];
                if(!list.contains(file))
                    list << file;

                line = s.readLine();
            }
        }

        if(line == "[aigroup_depot_typgroup_2]") {
            line = s.readLine();
            if(!list.contains(line))
                list << line;
        }
    }
    f.close();
    _checker->addToQueue(list, "ailists.cfg");
}

void OTMapScanner::scanTile(const QString &filename) {
    QStringList fileList;

    qDebug() << "reading " << filename;
    QFile f(_mapDir + "/" + filename);
    if(!f.exists()) {
        qWarning() << filename << " not found!";
        _missingTiles << filename;
        return;
    }

    if(!f.open(QFile::ReadOnly)) {
        qWarning() << "Could not open " << filename;
    }

    QTextStream s(&f);

    while(!s.atEnd()) {
        QString line = s.readLine();

        if(line == "[object]" || line == "[attachObj]" || line == "[splineAttachement]" || line == "[spline]" || line == "[spline_h]") {
            s.readLine();
            fileList << s.readLine();
        }

        if(line == "[splineAttachement_repeater]") {
            s.readLine();
            s.readLine();
            s.readLine();
            fileList << s.readLine();
        }
    }

    f.close();
    fileList.removeDuplicates();
    _checker->addToQueue(fileList, filename);
}

QList<OTFileSource> OTMapScanner::allTiles() const {
    return _allTiles;
}

QList<OTFileSource> OTMapScanner::allTextures() const {
    return _allTextures;
}

QStringList OTMapScanner::missingTiles() const {
    return _missingTiles;
}

QStringList OTMapScanner::missingTextures() const {
    return _missingTextures;
}

int OTMapScanner::allTilesCout() const {
    return _allTiles.count();
}

int OTMapScanner::allTexturesCount() const {
    return _allTextures.count();
}

int OTMapScanner::missingTilesCount() {
    return _missingTiles.count();
}

int OTMapScanner::missingTexturesCount() const {
    return _missingTextures.count();
}

OTFileSource *OTMapScanner::findOrCreateSourceObject(const QString &fileName, const bool &texture) {
    if(fileName.endsWith(".map")) {
        for(int i = 0; i < _allTiles.count(); i++)
            if(_allTiles[i].fileName() == fileName)
                return &_allTiles[i];

        _allTiles << OTFileSource(fileName, OTFileSource::TileFile);
        return &_allTiles.last();
    } else if(texture) {
        for(int i = 0; i < _allTextures.count(); i++)
            if(_allTextures[i].fileName() == fileName)
                return &_allTextures[i];

        _allTextures << OTFileSource(fileName, OTFileSource::TextureFile);
        return &_allTextures.last();
    }

    qCritical() << "Couldn't handle source object: " << fileName;

    return nullptr;
}
