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

    setWindowTitle(OTName + " - style test");

    // StyleTest stuff
    ui->listWidget->item(4)->setFlags(ui->listWidget->item(4)->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget->setCurrentRow(4);

    ui->page_7->setEnabled(false);
    ui->toolButton->addAction(ui->actionAction);
    ui->toolButton->addAction(ui->actionDisabledAction);

    auto* model2 = qobject_cast<QStandardItemModel*>(ui->comboBox->model());
    model2->item(4)->setEnabled(false);
    ui->comboBox->setCurrentIndex(4);

    connect(timer, &QTimer::timeout, this, &wStyleTest::loadStyleSheet);
    timer->start(1000);
    ui->ledStyle->setText(set.read(moduleName, "testStylesheet").toString());

    qInfo().noquote() << moduleName + " started successfully.";
}

wStyleTest::~wStyleTest()
{
    delete timer;
    delete ui;
}

void wStyleTest::on_horizontalSlider_valueChanged(int value)
{
    ui->progressBar->setValue(value);
}

void wStyleTest::loadStyleSheet()
{
    QFile themePath(ui->ledStyle->text());
    if (themePath.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&themePath);
        QString content = in.readAll();
        themePath.close();
        setStyleSheet(content);
    }

    set.write(moduleName, "testStylesheet", ui->ledStyle->text());
}

