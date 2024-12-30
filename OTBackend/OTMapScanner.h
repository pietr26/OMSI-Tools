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
    int missingHumansCount();
    int missingVehiclesCount() const;

protected:
    OTFileSource *findOrCreateSourceObject(const QString &fileName);

private:
    QString _omsiDir;
    QQueue<QList<QPair<QString,QString>>> _queue;
    QMutex _mutex;
    QWaitCondition _dataAvailable;
    bool _finish;

    QList<OTFileSource> _allSceneryobjects;
    QList<OTFileSource> _allSplines;
    QList<OTFileSource> _allHumans;
    QList<OTFileSource> _allVehicles;
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

    QList<OTFileSource> allTiles() const;
    QList<OTFileSource> allTextures() const;
    QStringList missingTiles() const;
    QStringList missingTextures() const;

    int allTilesCout() const;
    int allTexturesCount() const;
    int missingTilesCount();
    int missingTexturesCount() const;

protected:
    OTFileSource *findOrCreateSourceObject(const QString &fileName, const bool &texture = false);

signals:
    void initActionCount(int);
    void statusUpdate(int, QString);

private:
    OTMapChecker *_checker;
    QString _mapDir;
    QList<OTFileSource> _allTiles;
    QList<OTFileSource> _allTextures;
    QStringList _missingTiles;
    QStringList _missingTextures;
};



#endif // OTMAPSCANNER_H
