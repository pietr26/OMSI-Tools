#ifndef WDBPANEL_H
#define WDBPANEL_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTDatabaseHandler.h"
#include <QtSql>

// Function for time measurement:
//      f(x) = 0.0283x - 53.492

namespace Ui {
class wDBPanel;
}

class wDBPanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit wDBPanel(QWidget *parent = nullptr);
    ~wDBPanel();

private slots:

    void on_rbtnLink_clicked();

    void on_btnStart_clicked();

    void on_actionClose_triggered();

    void on_ledDirectory_textChanged(const QString &arg1);

    void on_lvwDuplicates_clicked(const QModelIndex &index);

    void on_btnSelectNew_clicked();

    void on_btnSelectOld_clicked();

    void on_btnOpenFolder_clicked();

    void on_rbtnStandardContent_clicked();

    void on_btnSelectAllNew_clicked();

    void on_btnSelectAllOld_clicked();

    void on_btnUnlockSource_clicked();

    void on_actionBackToHome_triggered();

signals:
    void backToHome();

private:
    Ui::wDBPanel *ui;

    OTSettings set;
    OTMiscellaneous misc;

    QStringList standardFilter;
    QStringListModel *strListModelDuplicates;
    QStringListModel *strListModelItems;

    int cutCount = set.read("main", "mainDir").toString().size() + 1;

    QString dbPath = "C:/Users/pietr/OneDrive/Dev/OMSI-Tools/OMSI-Tools/data/db/contentSearch.db";
    OTDatabaseHandler dbHandler;

    QList <QSqlQueryModel*> models;
    QStringList paths;

    QString link;
    QString currentLinkID;

    void removeCurrentFromList(bool all = false);
    void selectNew();
    void reloadSelectGroupBoxes();
    QString checkLinkID();

    void reloadTimeMeasurement(int remaining);
};

#endif // WDBPANEL_H
