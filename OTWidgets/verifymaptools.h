#ifndef VERIFYMAPTOOLS_H
#define VERIFYMAPTOOLS_H

#include <QWidget>
#include <QListWidget>
#include "OTModules/wVerifyMap/ignoreListFunctions.h"
#include "OTBackend/OTGlobal.h"
#include "OTModules/wContentSearch/wcontentsearch.h"

namespace Ui {
class verifyMapTools;
}

class verifyMapTools : public QWidget
{
    Q_OBJECT

public:
    explicit verifyMapTools(QListWidget *lwgParent, QWidget *parent);
    ~verifyMapTools();

private slots:
    void on_tbnTilesTools_clicked();

    void copy();

    void ignore();

    void search();

private:
    Ui::verifyMapTools *ui;
    OTMiscellaneous misc;
    OTSettings set;
    ignoreListFunctions iglF;
    wContentSearch* WCONTENTSEARCH;

    QListWidget *listWidgetParent;
};

#endif // VERIFYMAPTOOLS_H
