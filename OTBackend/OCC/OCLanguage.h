#ifndef OCLANGUAGE_H
#define OCLANGUAGE_H

#include "OCBase.h"

namespace OCLanguage
{

class Part
{
public:
    QString ident;
    QString translation;
};

class Language : public OCBase::File // olf
{
public:
    QString ident;
    QList<Part*> parts;
};

}

#endif // OCLANGUAGE_H
