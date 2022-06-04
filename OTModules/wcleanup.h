#ifndef WCLEANUP_H
#define WCLEANUP_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wCleanup;
}

class wCleanup : public QMainWindow
{
    Q_OBJECT

public:
    explicit wCleanup(QWidget *parent = nullptr);
    ~wCleanup();

private:
    Ui::wCleanup *ui;
    const QString moduleName = "wCleanup";

    OTSettings set;
};

#endif // WCLEANUP_H
