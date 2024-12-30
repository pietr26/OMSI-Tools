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
#include "OTFileSource.h"

class OTMapChecker : public QThread
{
    Q_OBJECT
public:
    explicit OTMapChecker(QObject *parent);
    void run() override;

    QString omsiDir() const;
    void setOmsiDir(const QString &);
    void addToQueue(const QList<QPair<QString, QString>> &);
    void addToQueue(const QStringList &, const QString &source);
    void setFinished();

    QList<OTFileSource> allSceneryobjects() const;
    QList<OTFileSource> allSplines() const;
    QList<OTFileSource> allHumans() const;
    QList<OTFileSource> allVehicles() const;
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
    int missingHumansCount() const;
    int missingVehiclesCount() const;
    int invalidSceneryobjectsCount() const;
    int invalidSplinesCount() const;
    int invalidHumansCount() const;
    int invalidVehiclesCount() const;

protected:
    OTFileSource *findOrCreateSourceObject(const QString &fileName, bool *wasNewCreated);

    void advancedCheck(OTFileSource *source);

private:
    QString _omsiDir;
    QQueue<QList<QPair<QString,QString>>> _queue;
    QMutex _mutex;
    QWaitCondition _dataAvailable;
    bool _finish;

    QHash<QString, OTFileSource> _allSceneryobjects;
    QHash<QString, OTFileSource> _allSplines;
    QHash<QString, OTFileSource> _allHumans;
    QHash<QString, OTFileSource> _allVehicles;
    QHash<QString, OTFileSource> _allUnkown;
    QStringList _missingSceneryobjects;
    QStringList _missingSplines;
    QStringList _missingHumans;
    QStringList _missingVehicles;
    QStringList _missingUnknown;
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

    QList<OTFileSource> allTiles() const;
    QList<OTFileSource> allTextures() const;
    QStringList missingTiles() const;
    QStringList missingTextures() const;

    int allTilesCout() const;
    int allTexturesCount() const;
    int missingTilesCount();
    int missingTexturesCount() const;
    int invalidTilesCount();
    int invalidTexturesCount() const;

protected:
    OTFileSource *findOrCreateSourceObject(const QString &fileName, const bool &texture = false);

signals:
    void initActionCount(int);
    void statusUpdate(int, QString);

private:
    OTMapChecker *_checker;
    QString _mapDir;
    QHash<QString, OTFileSource> _allTiles;
    QHash<QString, OTFileSource> _allTextures;
    QHash<QString, OTFileSource> _allUnkown;
    QStringList _missingTiles;
    QStringList _missingTextures;
    QStringList _missingUnknown;
};



#endif // OTMAPSCANNER_H
