#ifndef WRELEASENOTES_H
#define WRELEASENOTES_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wReleaseNotes;
}

class wReleaseNotes : public QMainWindow
{
    Q_OBJECT

public:
    explicit wReleaseNotes(QWidget *parent = nullptr, bool updateAvailable = false, QString newVersion = "");
    ~wReleaseNotes();

private slots:
    void downloadReleaseNotes();

    void on_btnClose_clicked();

    void on_btnUpdateNow_clicked();

    void on_cbxClearAppDir_stateChanged(int arg1);

private:
    Ui::wReleaseNotes *ui;
    OTSettings set;
    OTDownloader dl;
    OTMiscellaneous misc;
};

#endif // WRELEASENOTES_H
