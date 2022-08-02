#include "wstyletest.h"
#include "ui_wstyletest.h"

wStyleTest::wStyleTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wStyleTest)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    Q_UNUSED(parent);

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    resize(misc.sizeWindow(0.4, 1));
    qDebug() << "UI set";

    setWindowTitle(OTName + " - style test");

    // StyleTest stuff
    ui->statusbar->addPermanentWidget(ui->lblPermWidget1);
    ui->statusbar->addPermanentWidget(ui->lblPermWidget2);

    ui->listWidget->item(4)->setFlags(ui->listWidget->item(4)->flags() & ~Qt::ItemIsSelectable);
    ui->listWidget->setCurrentRow(4);

    ui->page_7->setEnabled(false);
    ui->toolButton->addAction(ui->actionAction);
    ui->toolButton->addAction(ui->actionDisabledAction);

    auto* model2 = qobject_cast<QStandardItemModel*>(ui->comboBox->model());
    model2->item(4)->setEnabled(false);
    ui->comboBox->setCurrentIndex(4);

    connect(&fileWatcher, &QFileSystemWatcher::fileChanged, this, &wStyleTest::loadStyleSheet);
    ui->ledStyle->setText(set.read(objectName(), "testStylesheet").toString());

    qInfo().noquote() << objectName() + " started";
}

wStyleTest::~wStyleTest()
{
    delete ui;
}

void wStyleTest::on_horizontalSlider_valueChanged(int value)
{
    ui->progressBar->setValue(value);
}

void wStyleTest::loadStyleSheet(QString file)
{
    Q_UNUSED(file);
    QFile themePath(ui->ledStyle->text());
    qDebug().noquote() << "Change style to:" << ui->ledStyle->text();

    if (QFileInfo(themePath).exists() && themePath.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&themePath);
        QString content = in.readAll();
        themePath.close();
        setStyleSheet(content);
    }
    else
        setStyleSheet("");
}

void wStyleTest::on_toolButton_clicked()
{
    ui->toolButton->showMenu();
}

void wStyleTest::on_btnSetThemeNone_clicked()
{
    ui->ledStyle->clear();
}

void wStyleTest::on_btnSetThemeCombinear_clicked()
{
    ui->ledStyle->setText("C:/Users/pietr/OneDrive/Dev/OMSI-Tools/OMSI-Tools/data/themes/Combinear.qss");
}

void wStyleTest::on_btnSetThemeDarkeum_clicked()
{
    ui->ledStyle->setText("C:/Users/pietr/OneDrive/Dev/OMSI-Tools/OMSI-Tools/data/themes/Darkeum.qss");
}

void wStyleTest::on_ledStyle_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    if (!fileWatcher.files().isEmpty())
        fileWatcher.removePaths(fileWatcher.files());

    if (!ui->ledStyle->text().isEmpty())
        fileWatcher.addPath(ui->ledStyle->text());

    set.write(objectName(), "testStylesheet", ui->ledStyle->text());
    loadStyleSheet(ui->ledStyle->text());
}


void wStyleTest::on_btnReloadStylePrefs_clicked()
{
    QFile modularStyle("C:/Users/pietr/OneDrive/Dev/OMSI-Tools/OMSI-Tools/data/themes/Dark_modular_test.qss");

    if (!modularStyle.open(QFile::ReadOnly | QFile::Text))
    {
        qWarning() << "Error while open modular style!";
        return;
    }

    QTextStream in(&modularStyle);

    QString style = in.readAll();
    style = style.replace("%<%Main%>%", ui->ledMain->text())
                 .replace("%<%MainSC%>%", ui->ledMainSC->text())
                 .replace("%<%Dis%>%", ui->ledDis->text())
                 .replace("%<%DisD%>%", ui->ledDisD->text())
                 .replace("%<%Acc1%>%", ui->ledAcc1->text())
                 .replace("%<%Acc2%>%", ui->ledAcc2->text())
                 .replace("%<%Acc3%>%", ui->ledAcc3->text())
                 .replace("%<%Button%>%", ui->ledButton->text());

    setStyleSheet(style);

    modularStyle.close();

}

