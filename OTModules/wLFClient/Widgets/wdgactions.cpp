#include "wdgactions.h"
#include "ui_wdgactions.h"

wdgActions::wdgActions(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wdgActions)
{
    ui->setupUi(this);

    updateButtons();
}

wdgActions::~wdgActions()
{
    delete ui;
}

void wdgActions::on_btnSpeak_clicked()
{
    ui->btnCrash->setChecked(false);
    updateButtons();
}


void wdgActions::on_btnCrash_clicked()
{
    ui->btnSpeak->setChecked(false);
    updateButtons();
}

void wdgActions::updateButtons()
{
    if (ui->btnSpeak->isChecked())
    {
        ui->btnSpeak->setStyleSheet("border: 3px solid #70AD47");
        ui->btnCrash->setStyleSheet("border: 3px dotted #C00000");

        // send speak 1
    }
    else if (ui->btnCrash->isChecked())
    {
        ui->btnSpeak->setStyleSheet("border: 3px dotted #70AD47");
        ui->btnCrash->setStyleSheet("border: 3px solid #C00000");

        // send speak 2
    }
    else
    {
        ui->btnSpeak->setStyleSheet("border: 3px dotted #70AD47");
        ui->btnCrash->setStyleSheet("border: 3px dotted #C00000");

        // send speak 0
    }
}

