#ifndef OCLANGUAGE_H
#define OCLANGUAGE_H

#include "OCBase.h"

class OCLanguage : public OCFile // olf
{
public:
    class Part
    {
    public:
        QString ident;
        QString translation;
    };

    QString ident;
    QList<Part> parts;
};

#endif // OCLANGUAGE_H
