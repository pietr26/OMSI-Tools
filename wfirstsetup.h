#ifndef WFIRSTSETUP_H
#define WFIRSTSETUP_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "wsettings.h"

namespace Ui {
class wFirstSetup;
}

class wFirstSetup : public QMainWindow
{
    Q_OBJECT

public:
    explicit wFirstSetup(QWidget *parent = nullptr);
    ~wFirstSetup();


private slots:
    void on_btnEn_clicked();

    void on_btnDe_clicked();

    void on_btnFr_clicked();

    void on_btnIt_clicked();

    void on_btnCz_clicked();

private:
    Ui::wFirstSetup *ui;
    OTSettings set;
    OTMiscellaneous misc;
    const QString moduleName = "wFirstSetup";
};

#endif // WFIRSTSETUP_H
