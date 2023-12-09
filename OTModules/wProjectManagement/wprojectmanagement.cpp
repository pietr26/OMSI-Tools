#include "wprojectmanagement.h"
#include "ui_wprojectmanagement.h"

wProjectManagement::wProjectManagement(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wProjectManagement)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());
    setWindowTitle(OTInformation::name + " - " + tr("Project management"));

    qInfo().noquote() << objectName() + " started";
}

wProjectManagement::~wProjectManagement()
{
    delete ui;
}

void wProjectManagement::on_actionBackToHome_triggered()
{
    close();
    emit backToHome();
}

void wProjectManagement::on_actionClose_triggered()
{
    close();
}

void wProjectManagement::execGit(QStringList args)
{
    QProcess gitProcess;
    gitProcess.setWorkingDirectory("C:/Users/pietr/Desktop/ot-test");

    // Beispiel: Git-Status abrufen
    gitProcess.start("git", args);
    gitProcess.waitForFinished();

    ui->pteOutputInfo->setPlainText(gitProcess.readAllStandardOutput());
    ui->pteOutputError->setPlainText(gitProcess.readAllStandardError());
}

void wProjectManagement::on_btnPullDownload_clicked()
{
    execGit(QStringList() << "XXX");
}

void wProjectManagement::on_btnFetchDownload_clicked()
{
    execGit(QStringList() << "XXX");
}

void wProjectManagement::on_btnCommitSave_clicked()
{
    execGit(QStringList() << "commit");
}

void wProjectManagement::on_btnPushUpload_clicked()
{
    execGit(QStringList() << "XXX");
}
