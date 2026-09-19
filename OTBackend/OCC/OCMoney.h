#ifndef OCMONEY_H
#define OCMONEY_H


#include <QList>
#include <QString>
#include "OCBase.h"

class OCMoney { // *.cfg
public:
    class moneyPart
    {
    public:
        QString modelFilename; // relative to CURRENT folder (without model folder!)
        float value = 0.0f;
    };

    QString name;
    int decimalCount = 0;

    QList<moneyPart> coins;
    QList<moneyPart> bills;
};

#endif // OCMONEY_H
