#include "firstsetup.h"
#include "ui_firstsetup.h"

firstSetup::firstSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::firstSetup)
{
    ui->setupUi(this);

    // Very simple: Copy from wpreferences.cpp, wPreferences::wPreferences():
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/en.svg"), OTStrings::langEn); // 0
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/de.svg"), OTStrings::langDe); // 1
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fr.svg"), OTStrings::langFr); // 2

    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/it.svg"), OTStrings::langIt); // 3

    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/cz.svg"), OTStrings::langCs); // 4

    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/et.svg"), OTStrings::langEt); // 5
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/eo.svg"), OTStrings::langEo); // 6
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fi.svg"), OTStrings::langFi); // 7
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/is.svg"), OTStrings::langIs); // 8
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/ja.svg"), OTStrings::langJa); // 9
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/cy.svg"), OTStrings::langCy); // 10

    // Disable languages:
    auto* model = qobject_cast<QStandardItemModel*>(ui->cobxLanguage->model());
    // model->item(0)->setEnabled(false); // en
    // model->item(1)->setEnabled(false); // de
    // model->item(2)->setEnabled(false); // fr
    model->item(3)->setEnabled(false); // it
    // model->item(4)->setEnabled(false); // cz
    model->item(5)->setEnabled(false); // et
    model->item(6)->setEnabled(false); // eo
    model->item(7)->setEnabled(false); // fi
    model->item(8)->setEnabled(false); // is
    model->item(9)->setEnabled(false); // ja
    model->item(10)->setEnabled(false); // cy
}

firstSetup::~firstSetup()
{
    delete ui;
}

/// Saves prefs and restarts application
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

