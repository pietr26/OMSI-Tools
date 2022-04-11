#ifndef WCHANGELOG_H
#define WCHANGELOG_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "wsettings.h"

namespace Ui {
class wChangelog;
}

class wChangelog : public QMainWindow
{
    Q_OBJECT

public:
    explicit wChangelog(QWidget *parent = nullptr, bool updateAvailable = false);
    ~wChangelog();

private slots:
    void downloadChangelog();

    void on_btnClose_clicked();

    void on_btnUpdateNow_clicked();

private:
    Ui::wChangelog *ui;
    const QString moduleName = "wChangelog";
    OTSettings set;
    OTDownloader dl;
    OTMiscellaneous misc;
    wSettings *WSETTINGS;
};

#endif // WCHANGELOG_H
