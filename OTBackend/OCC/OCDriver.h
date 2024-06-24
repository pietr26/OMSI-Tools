#ifndef OCDRIVER_H
#define OCDRIVER_H

#include "OCBase.h"

class OCDriver : public OCFile // odr
{
public:
    class BusInfo
    {
    public:
        QString friendlyname;
        int distance; // 100m (1 =^ 100m)
    };

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

    QList<BusInfo> busInfo;
};

#endif // OCDRIVER_H
