#ifndef WCHANGELOG_H
#define WCHANGELOG_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wChangelog;
}

class wChangelog : public QMainWindow
{
    Q_OBJECT

public:
    explicit wChangelog(QWidget *parent = nullptr, bool updateAvailable = false, QString newVersion = "");
    ~wChangelog();

private slots:
    void downloadChangelog();

    void on_btnClose_clicked();

    void on_btnUpdateNow_clicked();

    void on_cbxClearAppDir_stateChanged(int arg1);

private:
    Ui::wChangelog *ui;
    const QString moduleName = "wChangelog";
    OTSettings set;
    OTDownloader dl;
    OTMiscellaneous misc;
};

#endif // WCHANGELOG_H
