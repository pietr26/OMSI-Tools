#ifndef OCWEATHER_H
#define OCWEATHER_H


#include <QColor>
#include <QList>
#include <QString>
#include "OCBase.h"

class OCWeather
{
public:
    class Fog
    {
    public:
        int range = 0;
        int viewDistance; // 50000=unlimited
    };

    class Wind
    {
    public:
        float direction = 0.0f;
        float force = 0.0f;
    };

    class Temperature
    {
    public:
        float temperature = 0.0f;
        float dewPoint = 0.0f;
    };

    class Cloud
    {
    public:
        QString name;
        float height = 0.0f;
    };

    class Precip
    {
    public:
        enum Type
        {
            none = 0,
            rain = 1,
            snow = 2
        };

        Type type;
        int intensity = 0;
        int Precip3rdValue; // TODO: ?
        float Precip4thValue; // TODO: ?
        int Precip5thValue; // TODO: ?
    };

    class Groundwet
    {
    public:
            // TODO: ?
    };

    QString name;
    QString description;

    std::optional<Fog> fog;
    std::optional<Wind> wind;
    std::optional<Temperature> temperature;
    std::optional<float> pressure;
    std::optional<Cloud> clouds;
    std::optional<Precip> precip;

    std::optional<Groundwet> groundwet;

    bool snowOnRoad = false;
    bool snow = false;
};

class OCEnvir // envir.cfg
{
public:
    class SkyTextures
    {
    public:
        QString day;
        QString twilight;
        QString night;
    };

    class TwilightAngle
    {
    public:
        int start; // should be < 0
        int end; // should bei > 0
    };

    class Lightcolor // all RGB values!
    {
        QColor nadir;
        QColor twilightStart;
        QColor sunrise;
        QColor twilightEnd;
        QColor zenith;
    };

    SkyTextures skyTextures;
    TwilightAngle twilightAngle;

    Lightcolor lightcolorPrimary;
    Lightcolor lightcolorSecondary;
    Lightcolor lightcolorAmbient;
};

class OCClouds
{
public:
    class Cloudtype
    {
    public:
        enum Coverage
        {
            ovc = 0, // high coverage (overcast)
            sct = 1 // low coverage (scattered)
        };

        QString name;
        QString texture;
        int textureSize = 0;
        Coverage coverage;
    };

    QList<Cloudtype> clouds;
};

#endif // OCWEATHER_H
