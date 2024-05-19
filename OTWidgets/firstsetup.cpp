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

    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/cs.svg"), OTStrings::langCs); // 4
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fi.svg"), OTStrings::langFi); // 5
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/ja.svg"), OTStrings::langJa); // 6

    // Disable languages:
    auto* model = qobject_cast<QStandardItemModel*>(ui->cobxLanguage->model());
    // model->item(0)->setEnabled(false); // en
    // model->item(1)->setEnabled(false); // de
    // model->item(2)->setEnabled(false); // fr
    model->item(3)->setEnabled(false); // it
    // model->item(4)->setEnabled(false); // cs
    model->item(5)->setEnabled(false); // fi
    model->item(6)->setEnabled(false); // ja
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

