#ifndef OCVIEWABLE_H
#define OCVIEWABLE_H

#include "OCBase.h"

class OCViewable : public OCFile // Sceneryobjects, Vehicles, Humans
{
public:
    class Attachment
    {
    public:
        class Method
        {
        public:
            enum MethodIdent
            {
                xRotation = 1,
                yRotation = 2,
                zRotation = 3,
                transform = 4
            };

            MethodIdent type;
            float rotation;
            OC3DCoordinates<float> transformation;
        };
        QList<Method> methods;
    };

    QString friendlyname;
    QList<QString> groups;
    QString description;

    QList<Attachment> attachments;

    std::optional<OC3DBox<float>> boundingBox;

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

    std::optional<OC3DCoordinates<int>> momentOfIntertia;

    bool noDistanceCheck;
};

#endif // OCVIEWABLE_H
