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
        float singleRailLength;
        bool pushes;
        float waveLengthY;
        float maxAmplitudeY;
        float errorExponentY;

        float waveLengthZ;
        float maxAmplitudeZ;
        float errorExponentZ;
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
