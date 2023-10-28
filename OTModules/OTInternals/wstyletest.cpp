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

    setWindowTitle(OTInformation::name + " - style test");

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

/// Sets progress bar value
void wStyleTest::on_horizontalSlider_valueChanged(int value)
{
    ui->progressBar->setValue(value);
}

/// Loads a stylesheet
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

/// Shows toolbutton menu
void wStyleTest::on_toolButton_clicked()
{
    ui->toolButton->showMenu();
}

/// Sets no stylesheet
void wStyleTest::on_btnSetThemeNone_clicked()
{
    ui->ledStyle->clear();
}

/// Sets combinear theme (outdated!)
void wStyleTest::on_btnSetThemeCombinear_clicked()
{
    ui->ledStyle->setText("D:/OMSI-Tools/OMSI-Tools/data/themes/Combinear.qss");
}

/// Sets darkeum theme (outdated!)
void wStyleTest::on_btnSetThemeDarkeum_clicked()
{
    ui->ledStyle->setText("D:/OMSI-Tools/OMSI-Tools/data/themes/Darkeum.qss");
}

/// Saves test stylesheet and starts file watcher for changes
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

/// Set modular stylesheet
void wStyleTest::on_btnReloadStylePrefs_clicked()
{
    QFile modularStyle("D:/OMSI-Tools/OMSI-Tools/data/themes/modularModern.qss");

    if (!modularStyle.open(QFile::ReadOnly | QFile::Text))
    {
        qWarning() << "Error while open modular style!";
        return;
    }

    QTextStream in(&modularStyle);

    QString style = in.readAll();
    style = style.replace("%<%background%>%", ui->ledMain->text())
                .replace("%<%fontDisabled%>%", ui->ledFontDisabled->text())
                .replace("%<%backgroundDisabled%>%", ui->ledBackgroundDisabled->text())
                .replace("%<%borders%>%", ui->ledBorders->text())
                .replace("%<%accent%>%", ui->ledAccent->text())
                 .replace("%<%font%>%", ui->ledFont->text())
                 .replace("%<%inputs%>%", ui->ledButton->text());

    setStyleSheet(style);

    modularStyle.close();
}


void wStyleTest::on_actionClose_triggered()
{
    close();
}


void wStyleTest::on_actionBackToHome_triggered()
{
    close();
    backToHome();
}

