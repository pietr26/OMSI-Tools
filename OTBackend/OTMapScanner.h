#ifndef OTMAPSCANNER_H
#define OTMAPSCANNER_H

#include <QThread>
#include <QFile>
#include <QDir>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include <QDebug>
#include <QFile>
#include <QTextStream>

class OTMapChecker : public QThread
{
    Q_OBJECT
public:
    explicit OTMapChecker(QObject *parent);
    void run() override;

    QString omsiDir() const;
    void setOmsiDir(const QString &);
    void addToQueue(const QStringList &);
    void setFinished();

    QStringList allSceneryobjects() const;
    QStringList allSplines() const;
    QStringList allHumans() const;
    QStringList allVehicles() const;
    QStringList missingSceneryobjects() const;
    QStringList missingSplines() const;
    QStringList missingHumans() const;
    QStringList missingVehicles() const;

    int allSceneryobjectsCount() const;
    int allSplinesCount() const;
    int allHumansCount() const;
    int allVehiclesCount() const;
    int missingSceneryobjectsCount() const;
    int missingSplinesCount() const;
    int missingHumansCount();
    int missingVehiclesCount() const;

private:
    QString _omsiDir;
    QQueue<QStringList> _queue;
    QMutex _mutex;
    QWaitCondition _dataAvailable;
    bool _finish;

    QStringList _allSceneryobjects;
    QStringList _allSplines;
    QStringList _allHumans;
    QStringList _allVehicles;
    QStringList _missingSceneryobjects;
    QStringList _missingSplines;
    QStringList _missingHumans;
    QStringList _missingVehicles;
};

class OTMapScanner : public QThread
{
    Q_OBJECT
public:
    explicit OTMapScanner(QObject *parent, OTMapChecker *checker);
    void run() override;
    void setMapDir(const QString &);

    void scanGlobal();
    void scanTextures();
    void scanParkLists();
    void scanHumans();
    void scanAiList();
    void scanTile(const QString &filename);

    QStringList allTiles() const;
    QStringList allTextures() const;
    QStringList missingTiles() const;
    QStringList missingTextures() const;

    int allTilesCout() const;
    int allTexturesCount() const;
    int missingTilesCount();
    int missingTexturesCount() const;

signals:
    void initActionCount(int);
    void statusUpdate(int, QString);

private:
    OTMapChecker *_checker;
    QString _mapDir;
    QStringList _allTiles;
    QStringList _allTextures;
    QStringList _missingTiles;
    QStringList _missingTextures;
};



#endif // OTMAPSCANNER_H
