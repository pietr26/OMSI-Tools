#ifndef WFIRSTSETUP_H
#define WFIRSTSETUP_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include <QStandardItemModel>

namespace Ui {
class wFirstSetup;
}

class wFirstSetup : public QMainWindow
{
    Q_OBJECT

public:
    explicit wFirstSetup(QWidget *parent = nullptr);
    ~wFirstSetup();

private slots:
    void on_btnApply_clicked();

    void on_btnSelectMainDir_clicked();

    void on_cobxTheme_currentIndexChanged(int index);

private:
    Ui::wFirstSetup *ui;

    OTSettings set;
    OTMiscellaneous misc;
    OTStrings strings;

    QList<QPair<QString, QString>> themes = strings.getThemeNames();
};

#endif // WFIRSTSETUP_H
