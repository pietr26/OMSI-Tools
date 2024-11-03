#ifndef OCHUMAN_H
#define OCHUMAN_H

#include "OCBase.h"
#include "OCViewable.h"

namespace OCHuman
{

class OCHuman : public OCBase::File // hum
{
public:
    int age = -1;
    float seatHeight;
    float feetDistance;
    float height;

    OCType::Coord3D<float> hip; // xyz
    OCType::Coord3D<float> knee; // xyz
    OCType::Coord2DSide<float> waist; // yz
    OCType::Coord3D<float> shoulder; // xyz
    OCType::Coord3D<float> elbow; // xyz
    OCType::Coord2DSide<float> neck; // yz
    OCType::Coord3D<float> hand; // xyz
    OCType::Coord3D<float> finger; // xyz

    float stepWidth = 1.4;
    int upperArm = 66;
    float armSwingMultiplicator = 1;
    float walkHip = 1;
    float animWalkWaist = 0;
    QString ticketPackVoiceFolderName;

    OCViewable::Viewable model;
};

}

#endif // OCHUMAN_H
