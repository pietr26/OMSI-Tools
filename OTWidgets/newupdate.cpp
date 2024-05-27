#include "newupdate.h"
#include "ui_newupdate.h"

newUpdate::newUpdate(QPair<int, QString> update, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::newUpdate)
{
    ui->setupUi(this);

    updateInformation = update;
    ui->lblNewVersion->setText(updateInformation.second);
    ui->lblCurrentVersion->setText(OTInformation::versions::currentVersion.first);
}

newUpdate::~newUpdate()
{
    delete ui;
}

/// Opens wReleaseNotes to show latest update notes
void newUpdate::on_btnView_clicked()
{
    WRELEASENOTES = new wReleaseNotes(this, true, updateInformation.second, (updateInformation.first == 2) ? true : false);
    WRELEASENOTES->setWindowModality(Qt::ApplicationModal);
    WRELEASENOTES->show();
}

/// Calls a signal for opening the home screen
void newUpdate::on_btnDisard_clicked()
{
    emit goToStartScreen();
    setVisible(false);
}
