#ifndef OCVIEWABLE_H
#define OCVIEWABLE_H

#include "OCBase.h"

namespace OCViewable
{

class AttachmentMethod
{
public:
    enum Method
    {
        xRotationMethod = 1,
        yRotationMethod = 2,
        zRotationMethod = 3,
        transformMethod = 4
    };

    Method type;
    float rotation;
    OCType::Coord3D<float> transformation;
};

class Viewable : public OCBase::File // Sceneryobjects, Vehicles, Humans
{
public:
    QString friendlyname;
    QList<QString> groups;
    QString description;

    QList<QList<AttachmentMethod>> attachments;

    std::optional<OCType::Box<float>> boundingBox;

    QList<QString> varNameLists;
    QList<QString> stringVarNameLists;
    QList<QString> scripts;
    QList<QString> consfiles;

    QString modelPath;
    QString passengerCabinPath;
    QString pathPath;
    QString soundPath;
    QString aiSoundPath;

    std::optional<int> mass;

    std::optional<OCType::Coord3D<int>> momentOfIntertia;

    bool noDistanceCheck;
};

}

#endif // OCVIEWABLE_H
