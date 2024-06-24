#ifndef OCHUMAN_H
#define OCHUMAN_H

#include "OCBase.h"

class OCHuman : public OCFile // hum
{
public:
    int age = -1;
    float seatHeight;
    float feetDistance;
    float height;

    OC3DCoordinates<float> hip; // xyz
    OC3DCoordinates<float> knee; // xyz
    OC2DCoordinatesSide<float> waist; // yz
    OC3DCoordinates<float> shoulder; // xyz
    OC3DCoordinates<float> elbow; // xyz
    OC2DCoordinatesSide<float> neck; // yz
    OC3DCoordinates<float> hand; // xyz
    OC3DCoordinates<float> finger; // xyz

    float stepWidth = 1.4;
    int upperArm = 66;
    float armSwingMultiplicator = 1;
    float walkHip = 1;
    float animWalkWaist = 0;
    QString ticketPackVoiceFolderName;

    OCViewable model;
};

#endif // OCHUMAN_H
