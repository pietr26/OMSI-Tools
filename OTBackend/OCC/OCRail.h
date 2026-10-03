#ifndef OCRAIL_H
#define OCRAIL_H

#include "OCBase.h"

class OCRail
{
public:
    virtual void abstract() = 0;

    class RailEnh
    {
    public:
        float singleRailLength = 0.0f;
        bool pushes = false;
        float waveLengthY = 0.0f;
        float maxAmplitudeY = 0.0f;
        float errorExponentY = 0.0f;

        float waveLengthZ = 0.0f;
        float maxAmplitudeZ = 0.0f;
        float errorExponentZ = 0.0f;
    };

    class ThirdRail
    {
    public:
        int thirdRail1stValue; // TODO: ?
        int thirdRail2ndValue; // TODO: ?
        int thirdRail3rdValue; // TODO: ?
        int thirdRail4thValue; // TODO: ?
        int thirdRail5thValue; // TODO: ?
        int thirdRail6thValue; // TODO: ?
    };
};

#endif // OCRAIL_H
