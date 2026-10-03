#ifndef OCDRIVER_H
#define OCDRIVER_H


#include <QDate>
#include <QList>
#include <QString>
#include "OCBase.h"

namespace OCDriver
{

class BusInfo
{
public:
    QString friendlyname() const;
    void setFriendlyname(const QString &newFriendlyname);
    int distance() const;
    void setDistance(int newDistance);

private:
    QString _friendlyname;
    int _distance; // 100m (1 =^ 100m)
};

class Driver : public OCBase::File // odr
{
public:
    QString name;
    QString genderIdent;
    QDate birthDate;
    QDate hireDate;

    int totalBusstops = 0;
    int tooLateBusstops = 0;
    int tooEarlyBusstops = 0;

    int distance; // km

    int crashes = 0;
    int hitAndRun = 0;
    int extremeCrashes = 0;
    int injuredPeds = 0;

    int tickets = 0;
    float money = 0.0f;

    int passengerComfortTotal = 0;
    int passengerComfortPositive = 0;
    int ticketComfortTotal = 0;
    int ticketComfortPositive = 0;
    float driveComfort; // % decimal

    QList<BusInfo*> busInfo;
};

}

#endif // OCDRIVER_H
