#ifndef WMANAGEFILES_H
#define WMANAGEFILES_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wManageFiles;
}

class wManageFiles : public QMainWindow
{
    Q_OBJECT

public:
    explicit wManageFiles(QWidget *parent = nullptr);
    ~wManageFiles();

private:
    Ui::wManageFiles *ui;

    OTSettings set;
};

#endif // WMANAGEFILES_H
