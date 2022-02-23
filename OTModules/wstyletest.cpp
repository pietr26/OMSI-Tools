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
    adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTName + " - " + tr("settings"));
    //ui->statusbar->showMessage(tr("Restart to apply all settings."));

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    // StyleTest stuff
    auto* model = qobject_cast<QStandardItemModel*>(ui->listWidget->model());
    model->item(4)->setEnabled(false);
    ui->listWidget->setCurrentIndex(model->item(4)->index());

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

