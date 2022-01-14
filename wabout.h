#ifndef WABOUT_H
#define WABOUT_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wAbout;
}

class wAbout : public QMainWindow
{
    Q_OBJECT

public:
    explicit wAbout(QWidget *parent = nullptr);
    ~wAbout();

private:
    Ui::wAbout *ui;
    OTSettings set;
    const QString moduleName = "wAbout";
};

#endif // WABOUT_H
