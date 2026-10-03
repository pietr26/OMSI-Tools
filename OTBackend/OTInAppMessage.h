#ifndef OTINAPPMESSAGE_H
#define OTINAPPMESSAGE_H

#include <QDateTime>
#include <QString>
#include <QStringList>

class OTInAppMessage
{
public:
    QString ID;
    int publicity = 0;
    QDateTime start;
    QDateTime end;
    QString slug;
    QStringList versions;

    bool isImportant = false;
    bool isVirutal = false;

    QString enTitle;
    QString enShortDescription;
    QString enDescription;

    QString deTitle;
    QString deShortDescription;
    QString deDescription;

    QString image;
};

#endif // OTINAPPMESSAGE_H
