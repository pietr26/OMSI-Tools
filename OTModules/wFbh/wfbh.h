#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FbhFunctions.h"
#include "OTBackend/OTGlobal.h"
#include "OTModules/OTGeneric/wsettings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class wFbh;
}
QT_END_NAMESPACE

class wFbh : public QMainWindow
{
    Q_OBJECT

public:
    wFbh(QWidget *parent = nullptr);
    ~wFbh();

private slots:

    void on_actionClose_triggered();

    void on_ledName_textChanged(const QString &arg1);

    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

    void on_btnRemoveAll_clicked();

    void on_btnAddElement_clicked();

    void on_btnInputExampleReasons_clicked();

    void on_cobxWelcome_currentIndexChanged(int index);

    void on_btnFCopy_clicked();

    void on_btnFClear_clicked();

    void on_btnRCopy_clicked();

    void on_btnRClear_clicked();

    void on_cbxFCopyright_stateChanged(int arg1);

    void on_cbxFTrademark_stateChanged(int arg1);

    void on_btnFSelectAll_clicked();

    void on_lwgCopyrightItems_currentRowChanged(int currentRow);

    void on_actionSettings_triggered();

private:
    Ui::wFbh *ui;
    wSettings *WSETTINGS;
    OTSettings set;
    OTMiscellaneous misc;
    reasons r;

    QString language;
    QString de = "DEU";
    QString en = "ENG";

    bool finishedSetup = false;

    void getLanguage();

    QString urlParser(QString text, bool review = false);

    void setFVisible(bool visible);

    void setFooterSelection(int selectValue);

    int selectedFooter = 0;
};
#endif // WFBH_H
