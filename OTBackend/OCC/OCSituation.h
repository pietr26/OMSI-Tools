#ifndef OCSITUATION_H
#define OCSITUATION_H

#include "OCBase.h"

class OCSituation { // .osn
public:
    // For some situations there may be a weather file: [situationFileNameWithSuffix].owt

    class VehicleInformation
    {
    public:
        QString vehiclePath;
        OC3DCoordinates<float> position;
        float vehicle5thValue; // TODO: ?
        float vehicle6thValue; // TODO: ?
        float vehicle7thValue; // TODO: ?
        float vehicle8thValue; // TODO: ?
        float vehicle9thValue; // TODO: ?
        float vehicle10thValue; // TODO: ?
        float vehicle11thValue; // TODO: ?
        float vehicle12thValue; // TODO: ?
        int vehicle13thValue; // TODO: ?
        int vehicle14thValue; // TODO: ?
        QString hofFileName;

        bool isMyVehicle;

        QList<OCVariableDeclaration<float>> variables; // There are also variables with '' as name. - do not save int values (e.g. 1) as float (e.g. 1.0)! - [vars]
        QList<OCVariableDeclaration<QString>> stringvariables; // see 1st on member 'variables' - [stringvars]

        int coupledWith; // TODO: -1 actually in use?
    };

    class TimeTableInformation
    {
    public:
        QString line;
        QString tour;
        int settimetable3rdValue; // TODO: ?
        int settimetable4thValue; // TODO: ?
        int settimetable5thValue; // TODO: ?
        int settimetable6thValue; // TODO: ?
    };

    QString name;
    QString description;

    QString globalPath;
    QDateTime time; // year[int], daysFrom01Jan[int], hoursFromMidnight[int], minutesFromFullHour[int], secondsFromFullMinute[float]

    OC2DCoordinates<int> centerTile;
    OCMapPosition mapcam; // tilePos not used here! See 'centerTile' member
    OCMapPosition egoPosition; // tilePos not used here! See 'centerTile' member - std: x=10;y=10

    bool icaoWeatherActive;

    QList<VehicleInformation> vehicles;

    bool ttActive; // TODO: ?
    TimeTableInformation timeTableInformation;

    int myVehicle; // for OMSI itself, this variable has not any real sense
    int view; // TODO: / values maybe: 0=driver 1=passengers 2=outsideVehicle 3=outsideFull
};

#endif // OCSITUATION_H
