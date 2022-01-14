#ifndef WBUGREPOT_H
#define WBUGREPOT_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wBugRepot;
}

class wBugRepot : public QMainWindow
{
    Q_OBJECT

public:
    explicit wBugRepot(QWidget *parent = nullptr);
    ~wBugRepot();

private slots:
    void on_actionClose_triggered();

    void replyFinished(QNetworkReply *);

    void on_actionSend_triggered();

private:
    Ui::wBugRepot *ui;

    const QString moduleName = "wBugReport";
    OTSettings set;
};

#endif // WBUGREPOT_H
