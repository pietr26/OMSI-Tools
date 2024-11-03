#ifndef OCVEHICLE_H
#define OCVEHICLE_H

#include "OCBase.h"
#include "OCViewable.h"

class OCVehicle // bus, ovh
{
public:
    class View
    {
    public:
        enum Type
        {
            driver = 1,
            passenger = 2,
            reflexion = 3
        };

        Type type;
        OCType::Coord3D<float> position;
        float distanceFromOrigin;
        int perspective;
        float xRotation;
        float zRotation;

        // if (driverView) {
        bool isScheduleView;
        bool isTicketSellingView;
        // }

        // if (isReflexion) {
        bool isExtendedReflexion;
        // }
    };

    class Axis
    {
    public:
        class Method
        {
        public:
            enum AxisIdent
            {
                achse_long = 1,
                achse_maxwidth = 2,
                achse_minwidth = 3,
                achse_raddurchmesser = 4,
                achse_feder = 5,
                achse_maxforce = 6,
                achse_daempfer = 7,
                achse_antrieb = 8
            };

            int type;
            QVariant value;
        };

        QList<Method> methods;
    };

    class CouplingDefintion
    {
    public:
        class FrontCharacter
        {
        public:
            float alpha = -1; // Kickwinkel rechts-links
            float betaMin; // Knickwinkel Ri. unten
            float betaMax; // Knickwinkel Ri. oben
            float type; // 0=Truck (three degrees of freedom); 1=Bus (two degrees of freedom)
        };

        std::optional<OCType::Coord3D<float>> couplingFront; // coupling_front
        QString couplingFrontFile;  // couple_front
        bool couplingFrontIsReversed; // ...

        std::optional<FrontCharacter> frontCharacter;

        bool couplingFrontIsOpenForSound; // couple_front_open_for_sound

        std::optional<OCType::Coord3D<float>> couplingBack; // coupling_back
        QString couplingBackFile;   // couple_back
        bool couplingBackIsReversed; // ...
    };

    class Distance
    {
    public:
        int constructionYear;
        int kilometersPerYear;
    };

    class VehicleOscillating
    {
    public:
        float distanceBetweenBoogieAndCog; // z axis
        OCType::Coord3D<int> frequencies; // values: 2nd, 4th, 6th
        OCType::Coord3D<int> damping; // values: 3rd, 5th, 7th
    };

    class Registration
    {
    public:
        enum Method
        {
            automatic = 1,
            list = 2,
            free = 3
        };

        Method method;

        QString file;
        QString prefix;
        QString postfix;
    };

    class Sinus
    {
    public:
        float wheelRadius;
        float wheelDistance = 1.5;
        float wheelTaping;
        float damping;
    };

    class AiBrakePerformance
    {
    public:
        float meanBrakeDeceleration = 1.0;
        float rowdyFactorVariation = 0.2;
        float frictionUse = 0.3;
        float maxBreake = 0.2;
        float holingPointOffset = 0;
    };

    class ContactShoe // see Vehicles\A3\A3_64-66_K.ovh
    {
    public:
        enum Position
        {
            front = 0,
            rear = 1
        };

        Position position;
        float xMin;
        float xMax;
        float zMin;
        float zMax;
        int type; // +1=topContactRail +2=BottomContactRail +4=SideContactRail
    };

    class ControlCable // see Vehicles\A3\A3_64-66_K.ovh
    {
    public:
        enum Position
        {
            front = 1,
            back = 0
        };

        Position position;
        QString couplingPosition;
        int cablePosition;
        QString variabelRead;
        QString variableWrite;
        QString brokeCableVariable;
    };

    QList<View> driverViews;
    QList<View> passengerViews;

    int standardDriverView = -1;
    std::optional<OCType::Coord3D<float>> outsideCameraPosition;

    QList<View> reflexions;

    QList<OCViewable::AttachmentMethod> ticketAttachments;

    std::optional<float> gravityZPosition;
    std::optional<int> rollingResistance;
    std::optional<float> yRotationPoint;
    std::optional<float> steeringRadius; // [inv_min_turnradius] OR (trains) [boogie]?
    std::optional<float> aiDeltaHeight;
    std::optional<AiBrakePerformance> aiBrakePerformance;
    std::optional<int> aiVehicleType; // 0=passengerCar 1=cab 2=bus 3=truck

    QList<Axis> axes;

    CouplingDefintion coupling;

    std::optional<Distance> Distance;

    std::optional<OCType::Coord2D<float>> rowdyFactor;

    std::optional<VehicleOscillating> railBodyOscillating;

    OCViewable::Viewable object;
    bool scriptsharing;

    int type = -1; // TODO: ? - maybe see aiVehicleType (this class)

    QString numberFile;
    std::optional<Registration> registration;

    std::optional<Sinus> sinus;

    QList<ContactShoe> contactShoes;
};

#endif // OCVEHICLE_H
