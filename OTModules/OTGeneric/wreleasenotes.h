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
    explicit wReleaseNotes(QWidget *parent = nullptr, bool updateAvailable = false, QString newVersion = "", bool viaUpdater = false);
    ~wReleaseNotes();

private slots:
    void downloadReleaseNotes(QString version = "");

    void on_btnClose_clicked();

    void on_btnUpdateNow_clicked();

    void on_cbxBranch_currentIndexChanged(int index);

    void on_btnUpdateManually_clicked();

private:
    Ui::wReleaseNotes *ui;
    OTSettings set;
    OTNetworkConnection nc;
    OTMiscellaneous misc;

    bool setupFinished = false;
};

#endif // WRELEASENOTES_H
