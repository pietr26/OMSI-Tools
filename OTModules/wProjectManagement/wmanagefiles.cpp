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
    setWindowTitle(tr("Manage files"));

    qInfo().noquote() << objectName() + " started";
}

wManageFiles::~wManageFiles()
{
    delete ui;
}

// TODO: Use -f in git add:
//                              git add "file.end" -f
// ... to ignore the gitignore conventions
