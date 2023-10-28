#ifndef WDBCOPYRIGHTS_H
#define WDBCOPYRIGHTS_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTDatabaseHandler.h"
#include "OTModules/wFbh/waddpath.h"
#include "OTModules/wFbh/waddterm.h"
#include <QtSql>

namespace Ui {
class wDBCopyrights;
}

class wDBCopyrights : public QMainWindow
{
    Q_OBJECT

public:
    explicit wDBCopyrights(QWidget *parent = nullptr);
    ~wDBCopyrights();

private slots:

    void on_btnPathPreferencesAdd_clicked();

    void on_btnPathPreferencesRemove_clicked();

    void on_btnCopyrightTermsAdd_clicked();

    void on_btnCopyrightTermsRemove_clicked();

    void on_tvwCopyrightTerms_activated(const QModelIndex &index);

    void on_tvwPathPreferences_activated(const QModelIndex &index);

    void on_btnCreateBackup_clicked();

    void on_actionClose_triggered();

    void on_actionBackToHome_triggered();

signals:
    void backToHome();

private:
    Ui::wDBCopyrights *ui;

    OTSettings set;
    OTMiscellaneous misc;
    OTDatabaseHandler dbHandler;

    void updateView();
    void addTermFinished(int ID, QString argument);
    void addPathFinished(int ID, QString path, QString argumentIDs, int redirect);
};

#endif // WDBCOPYRIGHTS_H
