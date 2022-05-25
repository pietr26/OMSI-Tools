#include "wverifycationsettings.h"
#include "ui_wverifycationsettings.h"

wVerifycationSettings::wVerifycationSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wVerifycationSettings)
{
    ui->setupUi(this);
    ui->statusbar->setVisible(false);
    adjustSize();

    setWindowTitle(tr("Verifycation settings"));
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    // Set default settings
    if (!set.read(moduleName, "advVerifying").isValid())
        set.write(moduleName, "advVerifying", false);

    if (!set.read(moduleName, "onlyMapTextures").isValid())
        set.write(moduleName, "onlyMapTextures", false);

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    if (set.read(moduleName, "advVerifying").toInt() == 2)
        ui->cbxAdvancedVerifying->setChecked(true);

    if (set.read(moduleName, "onlyMapTextures") != "")
        ui->cbxOnlyMapTextures->setChecked(set.read(moduleName, "onlyMapTextures").toBool());

    if (ui->cbxAdvancedVerifying->isChecked())
        ui->cbxOnlyMapTextures->setEnabled(true);
    else
        ui->cbxOnlyMapTextures->setEnabled(false);
}

wVerifycationSettings::~wVerifycationSettings()
{
    delete ui;
}

void wVerifycationSettings::on_btnClose_clicked()
{
    close();
}

/// Sets the view to the selected verifying depth
void wVerifycationSettings::on_cbxAdvancedVerifying_stateChanged(int arg1)
{
    set.write(moduleName, "advVerifying", arg1);

    if (ui->cbxAdvancedVerifying->isChecked())
        ui->cbxOnlyMapTextures->setEnabled(true);
    else
        ui->cbxOnlyMapTextures->setEnabled(false);
}

void wVerifycationSettings::on_cbxOnlyMapTextures_stateChanged(int arg1)
{
    if (arg1 == 2)
        set.write(moduleName, "onlyMapTextures", true);
    else
        set.write(moduleName, "onlyMapTextures", false);
}
