#ifndef OCTRAIN_H
#define OCTRAIN_H

#include "OCBase.h"

class OCTrain
{
public:
    class TrainPart
    {
    public:
        QString trainPartPath;
        int zugFile2thValue; // TODO: ?
    };

    QList<TrainPart> trainParts;
};

#endif // OCTRAIN_H
