#ifndef OCTRANSLATION_H
#define OCTRANSLATION_H

#include "OCBase.h"

class OCTranslation : public OCBase::File // dsc
{
public:
    // TODO: Embed in other classes?
    QString name;
    QString description;
};

#endif // OCTRANSLATION_H
