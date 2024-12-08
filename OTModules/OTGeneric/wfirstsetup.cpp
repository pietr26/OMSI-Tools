#include "wfirstsetup.h"
#include "ui_wfirstsetup.h"

wFirstSetup::wFirstSetup(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wFirstSetup)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name);

    ui->lblInformation->setText(ui->lblInformation->text().arg(OTInformation::name));


    /// Very simple: Copy from wpreferences.cpp, wPreferences::wPreferences():

    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/en.svg"), OTStrings::langEn); // 0
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/de.svg"), OTStrings::langDe); // 1
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fr.svg"), OTStrings::langFr); // 2

    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/cs.svg"), OTStrings::langCs); // 3
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/nl.svg"), OTStrings::langNl); // 4
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fi.svg"), OTStrings::langFi); // 5
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/ja.svg"), OTStrings::langJa); // 6

    // Disable languages:
    auto* model = qobject_cast<QStandardItemModel*>(ui->cobxLanguage->model());
    // model->item(0)->setEnabled(false); // en
    // model->item(1)->setEnabled(false); // de
    // model->item(2)->setEnabled(false); // fr
    // model->item(3)->setEnabled(false); // cs
    // model->item(4)->setEnabled(false); // nl
    model->item(5)->setEnabled(false); // fi
    model->item(6)->setEnabled(false); // ja

    /// --------------------------------------------------------------------------------


    // cobxTheme:
    for (int i = 0; i < themes.count(); i++) ui->cobxTheme->addItem(themes[i].second);

    QString theme = set.read("main", "theme").toString();

    for (int i = 0; i < ui->cobxTheme->count(); i++)
    {
        if (themes[i].first == theme)
        {
            ui->cobxTheme->setCurrentIndex(i);
            break;
        }
    }
}

wFirstSetup::~wFirstSetup()
{
    delete ui;
}

void wFirstSetup::on_btnApply_clicked()
{
    set.write("main", "language", ui->cobxLanguage->currentIndex());
    set.write("main", "mainDir", ui->ledMainDir->text());
    set.write("main", "theme", themes[ui->cobxTheme->currentIndex()].first);

    misc.restart();
}

void wFirstSetup::on_btnSelectMainDir_clicked()
{
    QString mainDir = set.getOmsiPath(this);
    if (mainDir != "") ui->ledMainDir->setText(mainDir);
}

void wFirstSetup::on_cobxTheme_currentIndexChanged(int index)
{
    qApp->setStyle(QStyleFactory::create(themes[index].first));
}
