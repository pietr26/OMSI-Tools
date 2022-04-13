#ifndef WCONTENTSEARCH_H
#define WCONTENTSEARCH_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTDatabaseHandler.h"
#include "OTModules/OTGeneric/wsettings.h"

namespace Ui {
class wContentSearch;
}

class wContentSearch : public QMainWindow
{
    Q_OBJECT

public:
    explicit wContentSearch(QWidget *parent = nullptr, QStringList paths = QStringList());
    ~wContentSearch();

private slots:
    void on_actionSettings_triggered();

    void on_actionClose_triggered();

    void on_btnRemove_clicked();

    void on_actionRemoveSelection_triggered();

    void on_btnAddToList_clicked();

    void on_btnOpenInBrowser_clicked();

    void on_actionSearch_triggered();

    void on_btnSearch_clicked();

    void on_actionAddFile_triggered();

    void on_btnAdd_clicked();

    void on_btnAddList_clicked();

    void on_actionSendFeedback_triggered();

    void on_btnAddListToList_clicked();

    void on_btnCopy_clicked();

    void on_btnReportDeathLink_clicked();

    void on_btnAddFile_clicked();

    void on_btnClearLists_clicked();

    void on_lwgLinks_currentTextChanged(const QString &currentText);

private:
    Ui::wContentSearch *ui;
    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;
    wSettings *WSETTINGS;
    const QString moduleName = "wContentSearch";

    OTDatabaseHandler dbHandler;
    QString dbPath = ":/rec/contentSearchDatabases/current.db";
    QTemporaryFile database;
    void clearView(bool withoutUserInput = false);
    void reloadTabNames();
};

#endif // WCONTENTSEARCH_H
