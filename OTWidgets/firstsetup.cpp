#include "firstsetup.h"
#include "ui_firstsetup.h"

firstSetup::firstSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::firstSetup)
{
    ui->setupUi(this);

    // Very simple: Copy from wsettings.cpp, wSettings::wSettings():
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/en.svg"), OTStrings::langEn);
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/de.svg"), OTStrings::langDe);
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fr.svg"), OTStrings::langFr);
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/it.svg"), OTStrings::langIt);
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/cz.svg"), OTStrings::langCz);

    // Disable 'it'
    auto* model3 = qobject_cast<QStandardItemModel*>(ui->cobxLanguage->model());
    model3->item(3)->setEnabled(false);
}

firstSetup::~firstSetup()
{
    delete ui;
}

/// Saves settings and restarts application
void firstSetup::on_btnContinue_clicked()
{
    set.write("main", "language", ui->cobxLanguage->currentIndex());
    set.write("main", "mainDir", ui->ledMainDir->text());

    misc.restart();
}

/// Opens dialog for selectiin main directory
void firstSetup::on_btnSelectMainDir_clicked()
{
    QString mainDir = set.getOmsiPath(this);
    if (mainDir != "")
        ui->ledMainDir->setText(mainDir);
}

