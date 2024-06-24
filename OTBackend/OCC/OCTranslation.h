#ifndef OCTRANSLATION_H
#define OCTRANSLATION_H

#include "OCBase.h"

class OCTranslation : public OCFile // dsc
{
public:
    // TODO: Embed in other classes?
    QString name;
    QString description;
};

#endif // OCTRANSLATION_H
