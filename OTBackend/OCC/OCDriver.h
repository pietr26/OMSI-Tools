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

    QString name() const;
    void setName(const QString &newName);
    QString genderIdent() const;
    void setGenderIdent(const QString &newGenderIdent);
    QDate birthDate() const;
    void setBirthDate(const QDate &newBirthDate);
    QDate hireDate() const;
    void setHireDate(const QDate &newHireDate);
    int totalBusstops() const;
    void setTotalBusstops(int newTotalBusstops);
    int tooLateBusstops() const;
    void setTooLateBusstops(int newTooLateBusstops);
    int tooEarlyBusstops() const;
    void setTooEarlyBusstops(int newTooEarlyBusstops);
    int distance() const;
    void setDistance(int newDistance);
    int crashes() const;
    void setCrashes(int newCrashes);
    int hitAndRun() const;
    void setHitAndRun(int newHitAndRun);
    int extremeCrashes() const;
    void setExtremeCrashes(int newExtremeCrashes);
    int injuredPeds() const;
    void setInjuredPeds(int newInjuredPeds);
    int tickets() const;
    void setTickets(int newTickets);
    float money() const;
    void setMoney(float newMoney);
    int passengerComfortTotal() const;
    void setPassengerComfortTotal(int newPassengerComfortTotal);
    int passengerComfortPositive() const;
    void setPassengerComfortPositive(int newPassengerComfortPositive);
    int ticketComfortTotal() const;
    void setTicketComfortTotal(int newTicketComfortTotal);
    int ticketComfortPositive() const;
    void setTicketComfortPositive(int newTicketComfortPositive);
    float driveComfort() const;
    void setDriveComfort(float newDriveComfort);
    QList<BusInfo *> busInfo() const;
    void setBusInfo(const QList<BusInfo *> &newBusInfo);

private:
    QString _name;
    QString _genderIdent;
    QDate _birthDate;
    QDate _hireDate;

    int _totalBusstops;
    int _tooLateBusstops;
    int _tooEarlyBusstops;

    int _distance; // km

    int _crashes;
    int _hitAndRun;
    int _extremeCrashes;
    int _injuredPeds;

    int _tickets;
    float _money;

    int _passengerComfortTotal;
    int _passengerComfortPositive;
    int _ticketComfortTotal;
    int _ticketComfortPositive;
    float _driveComfort; // % decimal

    QList<BusInfo*> _busInfo;
};

}

#endif // OCDRIVER_H
