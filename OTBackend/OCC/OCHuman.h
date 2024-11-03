#ifndef OCHUMAN_H
#define OCHUMAN_H

#include "OCBase.h"
#include "OCViewable.h"

namespace OCHuman
{

class OCHuman : public OCBase::File // hum
{
public:
    int age() const;
    void setAge(int newAge);
    float seatHeight() const;
    void setSeatHeight(float newSeatHeight);
    float feetDistance() const;
    void setFeetDistance(float newFeetDistance);
    float height() const;
    void setHeight(float newHeight);

    OCType::Coord3D<float> hip() const;
    void setHip(const OCType::Coord3D<float> &newHip);
    OCType::Coord3D<float> knee() const;
    void setKnee(const OCType::Coord3D<float> &newKnee);
    OCType::Coord2DSide<float> waist() const;
    void setWaist(const OCType::Coord2DSide<float> &newWaist);
    OCType::Coord3D<float> shoulder() const;
    void setShoulder(const OCType::Coord3D<float> &newShoulder);
    OCType::Coord3D<float> elbow() const;
    void setElbow(const OCType::Coord3D<float> &newElbow);
    OCType::Coord2DSide<float> neck() const;
    void setNeck(const OCType::Coord2DSide<float> &newNeck);
    OCType::Coord3D<float> hand() const;
    void setHand(const OCType::Coord3D<float> &newHand);
    OCType::Coord3D<float> finger() const;
    void setFinger(const OCType::Coord3D<float> &newFinger);

    float stepWidth() const;
    void setStepWidth(float newStepWidth);
    int upperArm() const;
    void setUpperArm(int newUpperArm);
    float armSwingMultiplicator() const;
    void setArmSwingMultiplicator(float newArmSwingMultiplicator);
    float walkHip() const;
    void setWalkHip(float newWalkHip);
    float animWalkWaist() const;
    void setAnimWalkWaist(float newAnimWalkWaist);
    QString ticketPackVoiceFolderName() const;
    void setTicketPackVoiceFolderName(const QString &newTicketPackVoiceFolderName);

    OCViewable::Viewable model() const;
    void setModel(const OCViewable::Viewable &newModel);

private:
    int _age = -1;
    float _seatHeight;
    float _feetDistance;
    float _height;

    OCType::Coord3D<float> _hip; // xyz
    OCType::Coord3D<float> _knee; // xyz
    OCType::Coord2DSide<float> _waist; // yz
    OCType::Coord3D<float> _shoulder; // xyz
    OCType::Coord3D<float> _elbow; // xyz
    OCType::Coord2DSide<float> _neck; // yz
    OCType::Coord3D<float> _hand; // xyz
    OCType::Coord3D<float> _finger; // xyz

    float _stepWidth = 1.4;
    int _upperArm = 66;
    float _armSwingMultiplicator = 1;
    float _walkHip = 1;
    float _animWalkWaist = 0;
    QString _ticketPackVoiceFolderName;

    OCViewable::Viewable _model;
};

}

#endif // OCHUMAN_H
