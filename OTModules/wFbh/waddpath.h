#ifndef WADDPATH_H
#define WADDPATH_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTDatabaseHandler.h"
#include <QtSql>

namespace Ui {
class wAddPath;
}

class wAddPath : public QMainWindow
{
    Q_OBJECT

public:
    explicit wAddPath(bool isNewEntry, QWidget *parent = nullptr, int iD = -1, QString path = "", QString argumentIDs = "", int redirect = -1);
    ~wAddPath();

private slots:
    void on_cbxRedirect_stateChanged(int arg1);

    void on_btnSave_clicked();

    void on_ledPath_textChanged(const QString &arg1);

signals:
    void dialogFinished(int ID, QString path, QString argumentIDs, int redirect);

private:
    Ui::wAddPath *ui;
    OTSettings set;
    OTDatabaseHandler dbHandler;
};

#endif // WADDPATH_H
