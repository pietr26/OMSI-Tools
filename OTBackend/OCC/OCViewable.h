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

    Method type() const;
    void setType(Method newType);
    float rotation() const;
    void setRotation(float newRotation);
    OCType::Coord3D<float> transformation() const;
    void setTransformation(const OCType::Coord3D<float> &newTransformation);

private:
    Method _type;
    float _rotation;
    OCType::Coord3D<float> _transformation;
};

class Viewable : public OCBase::File // Sceneryobjects, Vehicles, Humans
{
private:
    QString _friendlyname;
    QList<QString> _groups;
    QString _description;

    QList<QList<AttachmentMethod>> _attachments;

    std::optional<OCType::Box<float>> _boundingBox;

    QList<QString> _varNameLists;
    QList<QString> _stringVarNameLists;
    QList<QString> _scripts;
    QList<QString> _consfiles;

    QString _modelPath;
    QString _passengerCabinPath;
    QString _pathPath;
    QString _soundPath;
    QString _aiSoundPath;

    std::optional<int> _mass;

    std::optional<OCType::Coord3D<int>> _momentOfIntertia;

    bool noDistanceCheck;
};

}

#endif // OCVIEWABLE_H
