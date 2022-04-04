#ifndef FIRSTSETUP_H
#define FIRSTSETUP_H

#include <QWidget>
#include "OTBackend/OTGlobal.h"
#include <QStandardItemModel>

namespace Ui {
class firstSetup;
}

class firstSetup : public QWidget
{
    Q_OBJECT

public:
    explicit firstSetup(QWidget *parent = nullptr);
    ~firstSetup();

private slots:


    void on_btnContinue_clicked();

private:
    Ui::firstSetup *ui;
    OTSettings set;
    OTMiscellaneous misc;
};

#endif // FIRSTSETUP_H
