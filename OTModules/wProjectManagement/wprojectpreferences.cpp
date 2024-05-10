#include "wprojectpreferences.h"
#include "ui_wprojectpreferences.h"

wProjectPreferences::wProjectPreferences(QString projectFolder, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wProjectPreferences)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    // Load prefs
    setWindowTitle(tr("Project preferences"));

    git.projectFolder = projectFolder;
    ui->ledMail->setText(git.exec(QStringList() << "config" << "user.email").first.trimmed());
    ui->ledName->setText(git.exec(QStringList() << "config" << "user.name").first.trimmed());
    ui->ledGitPath->setText(git.exec(QStringList() << "remote" << "get-url" << "origin").first.trimmed());

    qInfo().noquote() << objectName() + " started";
}

wProjectPreferences::~wProjectPreferences()
{
    delete ui;
}

void wProjectPreferences::on_btnSave_clicked()
{
    git.exec(QStringList() << "config" << "user.email" << ui->ledMail->text());
    git.exec(QStringList() << "config" << "user.name" << ui->ledName->text());
    git.exec(QStringList() << "remote" << "set-url" << "origin" << ui->ledGitPath->text());

    close();
}

void wProjectPreferences::on_btnClose_clicked()
{
    close();
}

