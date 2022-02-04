#ifndef DIGNORELIST_H
#define DIGNORELIST_H

#include <QDialog>
#include "OTBackend/OTGlobal.h"
#include "ignoreListFunctions.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class dIgnoreList;
}

class dIgnoreList : public QDialog
{
    Q_OBJECT

public:
    explicit dIgnoreList(QWidget *parent = nullptr);
    ~dIgnoreList();

private slots:

    void closeEvent (QCloseEvent *event);

    void on_btnCancel_clicked();

    void on_btnAddFiles_clicked();

    void on_btnRemove_clicked();

    void on_btnSave_clicked();

    void on_btnRemoveAll_clicked();

private:
    const QString moduleName = "dIgnoreList";
    Ui::dIgnoreList *ui;
    OTSettings set;
    OTMessage msg;
    ignoreListFunctions iglF;
    bool unsaved;

    void save();
};

#endif // DIGNORELIST_H
