#include "wcreateproject.h"
#include "ui_wcreateproject.h"

wCreateProject::wCreateProject(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wCreateProject)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());
    setWindowTitle(tr("Create project"));

    ui->btnCreate->setEnabled(false);

    qInfo().noquote() << objectName() + " started";
}

wCreateProject::~wCreateProject()
{
    delete ui;
}

void wCreateProject::on_btnSelectFolder_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, tr("Select project folder"), set.read(objectName(), "lastProjectFolder").toString().isEmpty() ? set.read("main", "mainDir").toString() : set.read(objectName(), "lastProjectFolder").toString());

    if (!folder.isEmpty()) ui->ledFolder->setText(folder);
}

void wCreateProject::on_btnCreate_clicked()
{
    // TODO: git setup
    // TODO: Create gitignore file

    emit creationFinished();
    close();
}

void wCreateProject::on_btnCancel_clicked()
{
    close();
}

void wCreateProject::on_ledName_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    verifyData();
}

void wCreateProject::on_ledFolder_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    verifyData();
}

bool wCreateProject::verifyData()
{
    git.projectFolder = ui->ledFolder->text().replace("/", "\\");
    QPair<QString, QString> status = git.exec(QStringList() << "rev-parse" << "--is-inside-work-tree");

    qInfo() << "gitOutput:" << status.first;
    qInfo() << "gitError:" << status.second;

    if (ui->ledName->text().isEmpty())
    {
        ui->btnCreate->setEnabled(false);
        return false;
    }
    else if (status.first.trimmed() == "true")
    {
        ui->btnCreate->setEnabled(false);
        ui->lblMessage->setText(tr("Selected project folder is already a project. To create a new one, delete the folder manually and retry creating a new project."));
        return false;
    }
    else
    {
        ui->btnCreate->setEnabled(true);
        ui->lblMessage->clear();
        return true;
    }
}
