#include "OCHuman.h"



namespace OCHuman {
int OCHuman::age() const
{
    return _age;
}

void OCHuman::setAge(int newAge)
{
    _age = newAge;
}

float OCHuman::seatHeight() const
{
    return _seatHeight;
}

void OCHuman::setSeatHeight(float newSeatHeight)
{
    _seatHeight = newSeatHeight;
}

float OCHuman::feetDistance() const
{
    return _feetDistance;
}

void OCHuman::setFeetDistance(float newFeetDistance)
{
    _feetDistance = newFeetDistance;
}

float OCHuman::height() const
{
    return _height;
}

void OCHuman::setHeight(float newHeight)
{
    _height = newHeight;
}

OCType::Coord3D<float> OCHuman::hip() const
{
    return _hip;
}

void OCHuman::setHip(const OCType::Coord3D<float> &newHip)
{
    _hip = newHip;
}

OCType::Coord3D<float> OCHuman::knee() const
{
    return _knee;
}

void OCHuman::setKnee(const OCType::Coord3D<float> &newKnee)
{
    _knee = newKnee;
}

OCType::Coord2DSide<float> OCHuman::waist() const
{
    return _waist;
}

void OCHuman::setWaist(const OCType::Coord2DSide<float> &newWaist)
{
    _waist = newWaist;
}

OCType::Coord3D<float> OCHuman::shoulder() const
{
    return _shoulder;
}

void OCHuman::setShoulder(const OCType::Coord3D<float> &newShoulder)
{
    _shoulder = newShoulder;
}

OCType::Coord3D<float> OCHuman::elbow() const
{
    return _elbow;
}

void OCHuman::setElbow(const OCType::Coord3D<float> &newElbow)
{
    _elbow = newElbow;
}

OCType::Coord2DSide<float> OCHuman::neck() const
{
    return _neck;
}

void OCHuman::setNeck(const OCType::Coord2DSide<float> &newNeck)
{
    _neck = newNeck;
}

OCType::Coord3D<float> OCHuman::hand() const
{
    return _hand;
}

void OCHuman::setHand(const OCType::Coord3D<float> &newHand)
{
    _hand = newHand;
}

OCType::Coord3D<float> OCHuman::finger() const
{
    return _finger;
}

void OCHuman::setFinger(const OCType::Coord3D<float> &newFinger)
{
    _finger = newFinger;
}

float OCHuman::stepWidth() const
{
    return _stepWidth;
}

void OCHuman::setStepWidth(float newStepWidth)
{
    _stepWidth = newStepWidth;
}

int OCHuman::upperArm() const
{
    return _upperArm;
}

void OCHuman::setUpperArm(int newUpperArm)
{
    _upperArm = newUpperArm;
}

float OCHuman::armSwingMultiplicator() const
{
    return _armSwingMultiplicator;
}

void OCHuman::setArmSwingMultiplicator(float newArmSwingMultiplicator)
{
    _armSwingMultiplicator = newArmSwingMultiplicator;
}

float OCHuman::walkHip() const
{
    return _walkHip;
}

void OCHuman::setWalkHip(float newWalkHip)
{
    _walkHip = newWalkHip;
}

float OCHuman::animWalkWaist() const
{
    return _animWalkWaist;
}

void OCHuman::setAnimWalkWaist(float newAnimWalkWaist)
{
    _animWalkWaist = newAnimWalkWaist;
}

QString OCHuman::ticketPackVoiceFolderName() const
{
    return _ticketPackVoiceFolderName;
}

void OCHuman::setTicketPackVoiceFolderName(const QString &newTicketPackVoiceFolderName)
{
    _ticketPackVoiceFolderName = newTicketPackVoiceFolderName;
}

OCViewable::Viewable OCHuman::model() const
{
    return _model;
}

void OCHuman::setModel(const OCViewable::Viewable &newModel)
{
    _model = newModel;
}

}
