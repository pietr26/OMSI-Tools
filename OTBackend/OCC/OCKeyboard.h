#ifndef OCKEYBOARD_H
#define OCKEYBOARD_H

#include "OCBase.h"

class OCKeyboard : public OCBase::File // cfg
{
public:
    class Entry
    {
    public:
        QString event;
        int keyIdent = -1; // TODO: std::optional?
        // bool ?;
    };


    QList<Entry> entries;
};

#endif // OCKEYBOARD_H
