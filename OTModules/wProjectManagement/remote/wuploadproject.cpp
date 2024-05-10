#include "wuploadproject.h"
#include "ui_wuploadproject.h"

wUploadProject::wUploadProject(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wUploadProject)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    // Load prefs
    setWindowTitle(tr("Upload project"));

    qInfo().noquote() << objectName() + " started";
}

wUploadProject::~wUploadProject()
{
    delete ui;
}

void wUploadProject::on_btnCancel_clicked()
{
    close();
}

