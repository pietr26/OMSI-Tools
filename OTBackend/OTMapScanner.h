#ifndef OTMAPSCANNER_H
#define OTMAPSCANNER_H

#include <QThread>
#include <QFile>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include <QDebug>
#include <QFile>
#include <QTextStream>

class OTMapScannerAbstract : public QThread
{
    Q_OBJECT
public:
    explicit OTMapScannerAbstract(QObject *parent) : QThread(parent) {}
    void run() override = 0;

signals:
    void initActionCount(int);
    void statusUpdate(int, QString);
private:
};

class OTMapChecker : public OTMapScannerAbstract
{
    Q_OBJECT
public:
    explicit OTMapChecker(QObject *parent);
    void run() override;

    void setOmsiDir(const QString &);
    void addToQueue(const QStringList &);
    void setFinished();

    QStringList allSceneryobjects() const;
    QStringList allSplines() const;
    QStringList allHumans() const;
    QStringList missingSceneryobjects() const;
    QStringList missingSplines() const;
    QStringList missingHumans() const;

    int allSceneryobjectsCount() const;
    int allSplinesCount() const;
    int allHumansCount() const;
    int missingSceneryobjectsCount() const;
    int missingSplinesCount() const;
    int missingHumansCount();

private:
    QString _omsiDir;
    QQueue<QStringList> _queue;
    QMutex _mutex;
    QWaitCondition _dataAvailable;
    bool _finish;

    QStringList _allSceneryobjects;
    QStringList _allSplines;
    QStringList _missingSceneryobjects;
    QStringList _missingSplines;
    QStringList _allHumans;
    QStringList _missingHumans;
};

class OTMapScanner : public OTMapScannerAbstract
{
    Q_OBJECT
public:
    explicit OTMapScanner(QObject *parent, OTMapChecker *checker);
    void run() override;
    void setMapDir(const QString &);

    void scanGlobal();
    void scanParkLists();
    void scanHumans();
    void scanTile(const QString &filename);

    QStringList allTiles() const;
    QStringList missingTiles() const;

    int allTilesCout() const;
    int missingTilesCount();

private:
    OTMapChecker *_checker;
    QString _mapDir;
    QStringList _allTiles;
    QStringList _missingTiles;
};



#endif // OTMAPSCANNER_H
