#ifndef OTFILEMETHODS_H
#define OTFILEMETHODS_H

#include <QFlags>

class OTFileMethods
{
public:
    enum methods
    {
        open = 1,
        reopen = 2,
        silentOpen = 3,
        save = 4,
        saveAs = 5,
        backupSave = 6
    };
    Q_DECLARE_FLAGS(fileMethods, methods)
};

#endif // OTFILEMETHODS_H
