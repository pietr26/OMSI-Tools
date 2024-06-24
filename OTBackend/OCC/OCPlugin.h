#ifndef OCPLUGIN_H
#define OCPLUGIN_H

#include "OCBase.h"

class OCPlugin // *.opl
{
public:
    QString dllName;
    QList<QString> varnames;
    QList<QString> stringvarnames;
    QList<QString> systemvarnames;
    QList<QString> triggers;
};

#endif // OCPLUGIN_H
