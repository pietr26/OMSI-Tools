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
    setStyleSheet(set.read("main", "theme").toString());
    setWindowTitle(tr("Project preferences"));

    git.projectFolder = projectFolder;
    ui->ledMail->setText(git.exec(QStringList() << "config" << "user.email").first.trimmed());
    ui->ledName->setText(git.exec(QStringList() << "config" << "user.name").first.trimmed());

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

    close();
}
