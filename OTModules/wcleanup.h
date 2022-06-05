#ifndef WCLEANUP_H
#define WCLEANUP_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTOmsiFileHandler.h"

namespace Ui {
class wCleanup;
}

class wCleanup : public QMainWindow
{
    Q_OBJECT

public:
    explicit wCleanup(QWidget *parent = nullptr);
    ~wCleanup();

private slots:

    void on_btnAnalyze_clicked();

private:
    Ui::wCleanup *ui;
    const QString moduleName = "wCleanup";

    OTSettings set;
    OTOMSIFileHandler filehandler;
};

#endif // WCLEANUP_H
