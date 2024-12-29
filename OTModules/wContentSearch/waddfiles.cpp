#include "waddfiles.h"
#include "ui_waddfiles.h"

wAddFiles::wAddFiles(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wAddFiles)
{
    ui->setupUi(this);
    setWindowTitle(tr("Add files"));
}

wAddFiles::~wAddFiles()
{
    delete ui;
}

void wAddFiles::on_btnAdd_clicked()
{
    QStringList paths;
    paths << QString(ui->ptePaths->toPlainText()).split("\n");

    paths.removeDuplicates();
    paths.removeAll("");
    paths.replaceInStrings("\\", "/");

    submitFiles(paths);
    hide();
}
