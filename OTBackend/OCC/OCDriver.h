#ifndef OCDRIVER_H
#define OCDRIVER_H

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

    int totalBusstops;
    int tooLateBusstops;
    int tooEarlyBusstops;

    int distance; // km

    int crashes;
    int hitAndRun;
    int extremeCrashes;
    int injuredPeds;

    int tickets;
    float money;

    int passengerComfortTotal;
    int passengerComfortPositive;
    int ticketComfortTotal;
    int ticketComfortPositive;
    float driveComfort; // % decimal

    QList<BusInfo*> busInfo;
};

}

#endif // OCDRIVER_H
