#ifndef WCONTENTSEARCH_H
#define WCONTENTSEARCH_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTDatabaseHandler.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTModules/wContentSearch/wsuggestfiles.h"
#include "OTModules/wContentSearch/waddfiles.h"

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
    void on_actionPreferences_triggered();

    void on_actionClose_triggered();

    void on_btnRemove_clicked();

    void on_actionRemoveSelection_triggered();

    void on_btnOpenInBrowser_clicked();

    void on_actionSearch_triggered();

    void on_btnSearch_clicked();

    void on_btnAddList_clicked();

    void on_actionSendFeedback_triggered();

    void on_btnCopy_clicked();

    void on_btnReportDeathLink_clicked();

    void on_btnAddFile_clicked();

    void on_btnClearLists_clicked();

    void on_lwgLinks_currentTextChanged(const QString &currentText);

    void on_actionSuggestFiles_triggered();

    void on_actionBackToHome_triggered();

    void on_actionAddExamples_triggered();

    void recieveSubmittedFiles(QStringList files);

signals:
    void backToHome();

private:
    Ui::wContentSearch *ui;
    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;
    wPreferences *WPREFERENCES;
    wSuggestFiles *WSUGGESTFILES;
    wAddFiles *WADDFILES;

    OTDatabaseHandler dbHandler;
    QString dbPath = ":/rec/data/db/contentSearch.db";
    QTemporaryFile database;
    void clearView(bool withoutUserInput = false);
    void reloadTabNames();
};

#endif // WCONTENTSEARCH_H
