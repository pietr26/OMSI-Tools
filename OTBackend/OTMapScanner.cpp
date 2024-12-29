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
    OTMapScannerAbstract(parent),
    _finish(false) {
}

void OTMapChecker::run() {
    _finish = false;
    _allSceneryobjects.clear();
    _allSplines.clear();
    _missingSceneryobjects.clear();
    _missingSplines.clear();

    while (true) {
        QMutexLocker locker(&_mutex);
        if (_queue.isEmpty()) {
            if (_finish) break;
            _dataAvailable.wait(&_mutex);
        }
        if (!_queue.isEmpty()) {
            QStringList files = _queue.dequeue();
            locker.unlock();

            for(QString file : files) {
                int type; //0 = Sceneryobject / 1 = Spline
                if(file.endsWith(".sco")) {
                    type = 0;
                } else if(file.endsWith(".sli")) {
                    type = 1;
                } else {
                    qWarning() << "Unkown file type" << file;
                    continue;
                }

                if(type == 0 && _allSceneryobjects.contains(file))
                    continue;

                if(type == 1 && _allSplines.contains(file))
                    continue;

                if(!QFile::exists(_omsiDir + "/" + file)) {
                    if(type == 0)
                        _missingSceneryobjects << file;
                    else
                        _missingSplines << file;
                }

                if(type == 0)
                    _allSceneryobjects << file;
                else
                    _allSplines << file;
            }
        }
    }

    // process stuff
    std::sort(_allSceneryobjects.begin(),     _allSceneryobjects.end());
    std::sort(_allSplines.begin(),             _allSplines.end());
    std::sort(_missingSceneryobjects.begin(), _missingSceneryobjects.end());
    std::sort(_missingSplines.begin(),        _missingSplines.end());
}

void OTMapChecker::setOmsiDir(const QString &str) {
    _omsiDir = str;
}

void OTMapChecker::addToQueue(const QStringList &list) {
    QMutexLocker locker(&_mutex);
    _queue.enqueue(list);
    _dataAvailable.wakeOne();
}

void OTMapChecker::setFinished() {
    QMutexLocker locker(&_mutex);
    _finish = true;
    _dataAvailable.wakeOne();
}

QStringList OTMapChecker::allSceneryobjects() const {
    return _allSceneryobjects;
}

QStringList OTMapChecker::allSplines() const {
    return _allSplines;
}

QStringList OTMapChecker::missingSceneryobjects() const {
    return _missingSceneryobjects;
}

QStringList OTMapChecker::missingSplines() const {
    return _missingSplines;
}

int OTMapChecker::allSceneryobjectsCount() const {
    return _allSceneryobjects.count();
}

int OTMapChecker::allSplinesCount() const {
    return _allSplines.count();
}

int OTMapChecker::missingSceneryobjectsCount() const {
    return _missingSceneryobjects.count();
}

int OTMapChecker::missingSplinesCount() const {
    return _missingSplines.count();
}

// ------------------------------------------------------------------------

OTMapScanner::OTMapScanner(QObject *parent, OTMapChecker *checker) :
    OTMapScannerAbstract(parent),
    _checker(checker)
{}

void OTMapScanner::run() {
    _allTiles.clear();
    _missingTiles.clear();
    scanGlobal();
    scanParkLists();

    int tileCount = _allTiles.count();
    emit initActionCount(tileCount);
    QString totalString = " /";

    int i = 1;
    for(QString str : _allTiles) {
        emit statusUpdate(i, tr("Read tile %1 of %2").arg(QString::number(i), QString::number(tileCount)));
        scanTile(str);
        i++;
    }

    _checker->setFinished();
}

void OTMapScanner::setMapDir(const QString &str) {
    _mapDir = str;
}

void OTMapScanner::scanGlobal() {
    QFile f(_mapDir + "/global.cfg");
    if(!f.exists())
        return;

    if(!f.open(QFile::ReadOnly))
        return;

    QTextStream s(&f);

    while(!s.atEnd()) {
        QString line = s.readLine();

        if(line == "[map]") {
            s.readLine();
            s.readLine();
            _allTiles << s.readLine();
        }
    }

    f.close();
}

void OTMapScanner::scanParkLists() {
    int i = 0;

    while(true) {
        QString indexStr = i != 0 ? "_" + QString::number(i) : "";
        QString path = _mapDir + "/parklist_p" + indexStr + ".txt";
        QFile f(path);
        if(!f.exists() && i != 0)
            break;
        else if(!f.exists()) {
            qWarning() << "parklist_p.txt not found!";
            break;
        }

        f.open(QFile::ReadOnly);
        QTextStream s(&f);
        QStringList list;
        while(!s.atEnd()) {
            list << s.readLine();
        }
        f.close();
        _checker->addToQueue(list);
        i++;
    }
}

void OTMapScanner::scanTile(const QString &filename) {
    QStringList fileList;

    QFile f(_mapDir + "/" + filename);
    if(!f.exists() || !f.open(QFile::ReadOnly)) {
        _missingTiles << filename;
        return;
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
    _checker->addToQueue(fileList);
}

QStringList OTMapScanner::allTiles() const {
    return _allTiles;
}

QStringList OTMapScanner::missingTiles() const {
    return _missingTiles;
}

int OTMapScanner::allTilesCout() const {
    return _allTiles.count();
}

int OTMapScanner::missingTilesCount() {
    return _missingTiles.count();
}
