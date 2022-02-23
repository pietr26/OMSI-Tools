#include "wstyletest.h"
#include "ui_wstyletest.h"

wStyleTest::wStyleTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wStyleTest)
{
    qInfo().noquote() << "Starting " + moduleName + "...";
    Q_UNUSED(parent);

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    resize(misc.sizeWindow(0.4, 1));
    qDebug() << "UI set";

    setWindowTitle(OTName + " - " + tr("style test"));

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    // StyleTest stuff
    ui->listWidget->item(4)->setFlags(ui->listWidget->item(4)->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget->setCurrentRow(4);

    ui->page_7->setEnabled(false);
    ui->tab_5->setEnabled(false);

    auto* model2 = qobject_cast<QStandardItemModel*>(ui->comboBox->model());
    model2->item(4)->setEnabled(false);
    ui->comboBox->setCurrentIndex(4);

    qInfo().noquote() << moduleName + " started successfully.";
}

wStyleTest::~wStyleTest()
{
    delete ui;
}

void wStyleTest::on_horizontalSlider_valueChanged(int value)
{
    ui->progressBar->setValue(value);
}

