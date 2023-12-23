#include "wmanagefiles.h"
#include "ui_wmanagefiles.h"

wManageFiles::wManageFiles(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wManageFiles)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());
    setWindowTitle(tr("Create project"));

    qInfo().noquote() << objectName() + " started";
}

wManageFiles::~wManageFiles()
{
    delete ui;
}
