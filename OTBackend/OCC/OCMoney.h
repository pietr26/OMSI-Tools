#ifndef OCMONEY_H
#define OCMONEY_H

#include "OCBase.h"

class OCMoney { // *.cfg
public:
    class moneyPart
    {
    public:
        QString modelFilename; // relative to CURRENT folder (without model folder!)
        float value;
    };

    QString name;
    int decimalCount;

    QList<moneyPart> coins;
    QList<moneyPart> bills;
};

#endif // OCMONEY_H
