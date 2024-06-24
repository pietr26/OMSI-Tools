#ifndef OCSOUND_H
#define OCSOUND_H

#include "OCBase.h"

class OCSound : public OCFile // in sound.cfg
{
public:
    class ThreeDimensionPosition
    {
    public:
        OC3DCoordinates<float> position;
        float maxIntensityRadius;
    };

    class Volcurve
    {
    public:
        QString variable;
        QList<OC2DCoordinates<float>> points;
    };

    template<class T>
    class Condition
    {
    public:
        enum Operation
        {
            notEqual = 0,
            equal = 1,
            lessThan = 2,
            greaterThan = 3,
            lessThanEqual = 4,
            greaterThanEqual = 5
        };

        static_assert(std::is_same_v<T, float> || std::is_same_v<T, int> || std::is_same_v<T, bool> || std::is_same_v<T, QVariant>, "The type T must be either float, int or bool.");
        QString variable;
        T value;
        Operation operation;
    };

    bool isLoopSound;
    float volume;
    // if (isLoopSound) {
    int sampleRate;
    QString pitchVariable;
    QString pitchVariableFactor; // TODO: Correct name?

    bool checkLoading;
    // }

    std::optional<ThreeDimensionPosition> position;

    int viewpoint = -1; // 0 equals 7   |   +1=userVehicleInside +2=userVehicleInside +4=aiVehicle

    // if (!isLoopSound) {
    bool noLoop;
    QString trigger;
    bool onlyOne; // TODO: ?
    // }

    // if (isLoopSound) {
    QList<Volcurve> volcurve;
    std::optional<Condition<QVariant>> condition;
    // }

    QVariant dir; // TODO: ? - direction?

    OCUnknown nextRandom; // TODO: ?
};

#endif // OCSOUND_H
