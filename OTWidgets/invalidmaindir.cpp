#include "invalidmaindir.h"
#include "ui_invalidmaindir.h"

invalidMainDir::invalidMainDir(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::invalidMainDir)
{
    ui->setupUi(this);
}

invalidMainDir::~invalidMainDir()
{
    delete ui;
}

void invalidMainDir::on_btnSelectNewPath_clicked()
{
    set.write("main", "mainDir", set.getOmsiPath(this, false));
    misc.restart();
}

void invalidMainDir::on_btnIgnore_clicked()
{
    goToStartScreen();
    setVisible(false);
}
