#ifndef WMAPS_H
#define WMAPS_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTOmsiFileHandler.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTModules/wmapselection.h"

namespace Ui {
class wMaps;
}

class wMaps : public QMainWindow
{
    Q_OBJECT

public:
    explicit wMaps(QWidget *parent = nullptr);
    ~wMaps();

signals:
    void backToHome();

private slots:
    void on_actionPreferences_triggered();

    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

    void on_actionSendFeedback_triggered();

    void on_actionLoadMap_triggered();

    void on_btnLoadMap_clicked();

    void recieveSelectedMap(QPair<QString, QString> mapInfo);

private:
    Ui::wMaps *ui;

    OTMessage msg;
    OTSettings set;
    OTMiscellaneous misc;
    OTOMSIFileHandler filehandler;
    wPreferences *WPREFERENCES;
    wMapSelection *WMAPSELECTION;

    QPair<QString, QString> currentMap;
};

#endif // WMAPS_H
