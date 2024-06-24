#ifndef OCTEXTURECHANGEMASTER_H
#define OCTEXTURECHANGEMASTER_H

#include "OCBase.h"

class OCTextureChangeMaster
{
public:
    QString filename;
    QString variable; // int variable!

    QList<QString> entries;
};

#endif // OCTEXTURECHANGEMASTER_H
