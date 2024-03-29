#ifndef WBUGDOC_H
#define WBUGDOC_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTExternal.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "windows.h"
#include <QPrinter>

namespace Ui {
class wBugDoc;
}

class wBugDoc : public QMainWindow
{
    Q_OBJECT

public:
    explicit wBugDoc(QWidget *parent = nullptr);
    ~wBugDoc();

signals:
    void backToHome();

private slots:
    void on_actionBack_to_home_triggered();

    void on_actionClose_triggered();

    void on_actionLoad_triggered();

    void on_tvwBugs_activated(const QModelIndex &index);

    void on_ledID_textChanged(const QString &arg1);

    void on_ledTitle_textChanged(const QString &arg1);

    void on_pteDescription_textChanged();

    void on_ledLocation_textChanged(const QString &arg1);

    void on_ledPicturePath_textChanged(const QString &arg1);

    void on_btnDelete_clicked();

    void on_btnAdd_clicked();

    void on_btnWait_clicked();

    void on_actionHTML_triggered();

    void on_actionPreferences_triggered();

    void on_btnWebDiskCopy_clicked();

    void on_btnCopyPicture_clicked();

private:
    Ui::wBugDoc *ui;
    wPreferences *WPREFERENCES;

    void loadUI();

    void loadSingleItem();

    void clearSingleBug();

    void loadPicture();

    void save();

    bool noAutoSave = false;

    OTMessage msg;
    OTSettings set;
    OTMiscellaneous misc;
    OTNetworkConnection nc;

    OTDatabaseHandler dbHandler;

    QString projectFolder = "";
};

#endif // WBUGDOC_H
