#include "OCDriver.h"

namespace OCDriver {

QString BusInfo::friendlyname() const
{
    return _friendlyname;
}

void BusInfo::setFriendlyname(const QString &newFriendlyname)
{
    _friendlyname = newFriendlyname;
}

int BusInfo::distance() const
{
    return _distance;
}

void BusInfo::setDistance(int newDistance)
{
    _distance = newDistance;
}

QString Driver::name() const
{
    return _name;
}

void Driver::setName(const QString &newName)
{
    _name = newName;
}

QString Driver::genderIdent() const
{
    return _genderIdent;
}

void Driver::setGenderIdent(const QString &newGenderIdent)
{
    _genderIdent = newGenderIdent;
}

QDate Driver::birthDate() const
{
    return _birthDate;
}

void Driver::setBirthDate(const QDate &newBirthDate)
{
    _birthDate = newBirthDate;
}

QDate Driver::hireDate() const
{
    return _hireDate;
}

void Driver::setHireDate(const QDate &newHireDate)
{
    _hireDate = newHireDate;
}

int Driver::totalBusstops() const
{
    return _totalBusstops;
}

void Driver::setTotalBusstops(int newTotalBusstops)
{
    _totalBusstops = newTotalBusstops;
}

int Driver::tooLateBusstops() const
{
    return _tooLateBusstops;
}

void Driver::setTooLateBusstops(int newTooLateBusstops)
{
    _tooLateBusstops = newTooLateBusstops;
}

int Driver::tooEarlyBusstops() const
{
    return _tooEarlyBusstops;
}

void Driver::setTooEarlyBusstops(int newTooEarlyBusstops)
{
    _tooEarlyBusstops = newTooEarlyBusstops;
}

int Driver::distance() const
{
    return _distance;
}

void Driver::setDistance(int newDistance)
{
    _distance = newDistance;
}

int Driver::crashes() const
{
    return _crashes;
}

void Driver::setCrashes(int newCrashes)
{
    _crashes = newCrashes;
}

int Driver::hitAndRun() const
{
    return _hitAndRun;
}

void Driver::setHitAndRun(int newHitAndRun)
{
    _hitAndRun = newHitAndRun;
}

int Driver::extremeCrashes() const
{
    return _extremeCrashes;
}

void Driver::setExtremeCrashes(int newExtremeCrashes)
{
    _extremeCrashes = newExtremeCrashes;
}

int Driver::injuredPeds() const
{
    return _injuredPeds;
}

void Driver::setInjuredPeds(int newInjuredPeds)
{
    _injuredPeds = newInjuredPeds;
}

int Driver::tickets() const
{
    return _tickets;
}

void Driver::setTickets(int newTickets)
{
    _tickets = newTickets;
}

float Driver::money() const
{
    return _money;
}

void Driver::setMoney(float newMoney)
{
    _money = newMoney;
}

int Driver::passengerComfortTotal() const
{
    return _passengerComfortTotal;
}

void Driver::setPassengerComfortTotal(int newPassengerComfortTotal)
{
    _passengerComfortTotal = newPassengerComfortTotal;
}

int Driver::passengerComfortPositive() const
{
    return _passengerComfortPositive;
}

void Driver::setPassengerComfortPositive(int newPassengerComfortPositive)
{
    _passengerComfortPositive = newPassengerComfortPositive;
}

int Driver::ticketComfortTotal() const
{
    return _ticketComfortTotal;
}

void Driver::setTicketComfortTotal(int newTicketComfortTotal)
{
    _ticketComfortTotal = newTicketComfortTotal;
}

int Driver::ticketComfortPositive() const
{
    return _ticketComfortPositive;
}

void Driver::setTicketComfortPositive(int newTicketComfortPositive)
{
    _ticketComfortPositive = newTicketComfortPositive;
}

float Driver::driveComfort() const
{
    return _driveComfort;
}

void Driver::setDriveComfort(float newDriveComfort)
{
    _driveComfort = newDriveComfort;
}

QList<BusInfo *> Driver::busInfo() const
{
    return _busInfo;
}

void Driver::setBusInfo(const QList<BusInfo *> &newBusInfo)
{
    _busInfo = newBusInfo;
}

}
