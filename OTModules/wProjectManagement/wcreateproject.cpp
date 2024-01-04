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

    nameValidator = new QRegularExpressionValidator(QRegularExpression("[0-9A-Za-z._-]*"), ui->ledName);

    ui->ledName->setValidator(nameValidator);

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
    QFile readme(ui->ledFolder->text() + "/" + "README.md");
    git.projectFolder = ui->ledFolder->text();

    if (!readme.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::critical(this, tr("Could not create repository"), tr("The initial README.md file could not be created."));
    }
    else
    {
        QTextStream readmeIn(&readme);
        readmeIn << "# " << ui->ledName;
        readme.close();

        QFile::copy(":/rec/data/base-gitignore.txt", ui->ledFolder->text() + "/.gitignore");

        // TODO: Create gitignore file

        qInfo() << git.exec(QStringList() << "init");
        qInfo() << git.exec(QStringList() << "git add .");
        qInfo() << git.exec(QStringList() << "commit" << "-m" << tr("First commit"));
        qInfo() << git.exec(QStringList() << "branch" << "-M" << "main");

        emit creationFinished(git.projectFolder);
        close();

        /*
git add README.md
  git commit -m "first commit"
  git branch -M main
  git remote add origin https://github.com/pietr26/ot-test.git
  git push -u origin main
     */
    }
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

    if (ui->ledName->text().isEmpty() || ui->ledFolder->text().isEmpty())
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
