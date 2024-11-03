#include "OCViewable.h"

namespace OCViewable {

AttachmentMethod::Method AttachmentMethod::type() const
{
    return _type;
}

void AttachmentMethod::setType(Method newType)
{
    _type = newType;
}

float AttachmentMethod::rotation() const
{
    return _rotation;
}

void AttachmentMethod::setRotation(float newRotation)
{
    _rotation = newRotation;
}

OCType::Coord3D<float> AttachmentMethod::transformation() const
{
    return _transformation;
}

void AttachmentMethod::setTransformation(const OCType::Coord3D<float> &newTransformation)
{
    _transformation = newTransformation;
}

}
