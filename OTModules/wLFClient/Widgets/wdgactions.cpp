#include "wdgactions.h"
#include "ui_wdgactions.h"

wdgActions::wdgActions(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wdgActions)
{
    ui->setupUi(this);
}

wdgActions::~wdgActions()
{
    delete ui;
}

void wdgActions::on_btnSpeak_clicked()
{
    ui->btnCrash->setChecked(false);
    sendStatus();
}


void wdgActions::on_btnCrash_clicked()
{
    ui->btnSpeak->setChecked(false);
    sendStatus();
}

void wdgActions::sendStatus()
{
    if (ui->btnSpeak->isChecked())
    {
        // send speak 1
    }
    else if (ui->btnCrash->isChecked())
    {
        // send speak 2
    }
    else
    {
        // send speak 0
    }
}

