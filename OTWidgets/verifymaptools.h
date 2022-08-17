#ifndef VERIFYMAPTOOLS_H
#define VERIFYMAPTOOLS_H

#include <QWidget>
#include <QListWidget>
#include "OTBackend/OTGlobal.h"
#include "OTModules/wContentSearch/wcontentsearch.h"

namespace Ui {
class verifyMapTools;
}

class verifyMapTools : public QWidget
{
    Q_OBJECT

public:
    explicit verifyMapTools(QListWidget *lwgAllParent, QListWidget *lwgMissingParent, QWidget *parent);
    ~verifyMapTools();

private slots:

    void on_tbnCopy_clicked();

    void on_tbnSearch_clicked();

    void on_actionCopyFromAll_triggered();

    void on_actionCopyFromMissing_triggered();

    void on_actionSearchFromAll_triggered();

    void on_actionSearchFromMissing_triggered();

private:
    Ui::verifyMapTools *ui;
    OTMiscellaneous misc;
    OTSettings set;
    wContentSearch* WCONTENTSEARCH;

    QListWidget *listWidgetAllParent;
    QListWidget *listWidgetMissingParent;

    void copy(QListWidget *lwg);

    void search(QListWidget *lwg);
};

#endif // VERIFYMAPTOOLS_H
