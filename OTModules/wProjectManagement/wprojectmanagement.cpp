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

    enableUi(false);
    clearUi();

    connect(fileWatcherTimer, &QTimer::timeout, this, &wProjectManagement::getGitStatus);

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

void wProjectManagement::restartFileWatcherTimer()
{
    fileWatcherTimer->stop();
    fileWatcherTimer->start(2000);
}

void wProjectManagement::enableUi(bool enable)
{
    ui->centralwidget->setEnabled(enable);
    ui->actionProjectSettings->setEnabled(enable);
}

void wProjectManagement::clearUi()
{
    ui->twgStatus->clear();
    ui->twgStatus->setHorizontalHeaderLabels(QStringList() << tr("Status") << tr("File"));
    ui->twgStatus->setRowCount(0);

    ui->pteOutputInfo->clear();
    ui->pteOutputError->clear();
}

bool wProjectManagement::checkFolderStatus(bool newProject)
{
    if (git.projectFolder.isEmpty()) return false;

    QPair<QString, QString> status = git.exec(QStringList() << "rev-parse" << "--is-inside-work-tree");

    if ((status.first.trimmed() != "true") && !newProject)
    {
        QMessageBox::warning(this, "Repo not exists", "Dir is not a repo!");
        return false;
    }
    else if ((status.first.trimmed() == "true") && newProject)
    {
        QMessageBox::warning(this, "Dir is repo", "The directory is already a repo!");
        return false;
    }

    return true;
}

void wProjectManagement::loadProject()
{
    if (!git.projectFolder.isEmpty())
    {
        clearUi();
        enableUi(false);

        set.write(objectName(), "lastProjectFolder", git.projectFolder);

        getGitStatus();

        watcher = new QFileSystemWatcher(QStringList() << git.projectFolder);
        QObject::connect(watcher, &QFileSystemWatcher::fileChanged, this, &wProjectManagement::restartFileWatcherTimer);
        QObject::connect(watcher, &QFileSystemWatcher::directoryChanged, this, &wProjectManagement::restartFileWatcherTimer);

        enableUi(true);
    }
}

void wProjectManagement::on_btnPullDownload_clicked()
{
    git.exec(QStringList() << "XXX");
}

void wProjectManagement::on_btnFetchDownload_clicked()
{
    git.exec(QStringList() << "XXX");
}

void wProjectManagement::on_btnCommitSave_clicked()
{
    // TODO: Identity check
    QPair<QString, QString> result = git.exec(QStringList() << "commit");
}

void wProjectManagement::on_btnPushUpload_clicked()
{
    git.exec(QStringList() << "XXX");
}

void wProjectManagement::selectProjectFolder(bool newProject = false)
{
    git.projectFolder = QFileDialog::getExistingDirectory(this, tr("Select project folder"), set.read(objectName(), "lastProjectFolder").toString().isEmpty() ? set.read("main", "mainDir").toString() : set.read(objectName(), "lastProjectFolder").toString());

    if (!checkFolderStatus(newProject))
    {
        clearUi();
        enableUi(false);
        return;
    }

    loadProject();
}

void wProjectManagement::getGitStatus()
{
    fileWatcherTimer->stop();
    qInfo() << "getGitStatus!";
    QList<QPair<QString, QString>> status;

    QStringList output = git.exec(QStringList() << "status" << "--short").first.split("\n");

    for (int i = 0; i < output.count(); i++)
    {
        QChar ident = (output[i].at(0) == ' ') ? output[i].at(1) : output[i].at(0);

             if (ident == 'M') status << QPair<QString, QString>(tr("Modified"),                output[i].remove(0, 3));
        else if (ident == 'A') status << QPair<QString, QString>(tr("Added"),                   output[i].remove(0, 3)); // will not happen due to user optimization
        else if (ident == 'D') status << QPair<QString, QString>(tr("Deleted"),                 output[i].remove(0, 3));
        else if (ident == 'R') status << QPair<QString, QString>(tr("Renamed"),                 output[i].remove(0, 3)); // TODO: with QFileWatcher?
        else if (ident == 'C') status << QPair<QString, QString>(tr("Copied"),                  output[i].remove(0, 3)); // TODO: ?
        else if (ident == 'U') status << QPair<QString, QString>(tr("Updated but unmerged"),    output[i].remove(0, 3)); // TODO: ?
        else if (ident == '?') status << QPair<QString, QString>(tr("Unstaged"),                output[i].remove(0, 3));
      //else                   status << QPair<QString, QString>("???",                         output[i].remove(0, 3));
    }

    for (int i = 0; i < status.count(); i++)
    {
        ui->twgStatus->setRowCount(status.count());

        for (int i = 0; i < status.count(); i++)
        {
            ui->twgStatus->setItem(i, 0, new QTableWidgetItem(status[i].first));
            ui->twgStatus->item(i, 0)->setCheckState(Qt::Unchecked);
            ui->twgStatus->setItem(i, 1, new QTableWidgetItem(status[i].second));
        }

        ui->twgStatus->resizeColumnsToContents();
    }

    // for (int i = 0; i < status.count(); i++)
    // {
    //     modelStatus = new QStandardItemModel(status.count(), 2);
    //     modelStatus->setHeaderData(0, Qt::Horizontal, tr("Status"));
    //     modelStatus->setHeaderData(1, Qt::Horizontal, tr("File"));
    //     for (int i = 0; i < status.count(); i++)
    //     {
    //         modelStatus->setData(modelStatus->index(i, 0), status[i].first, Qt::ItemIsUserCheckable);
    //         modelStatus->setData(modelStatus->index(i, 1), status[i].second);
    //     }

    //     ui->tvwStatus->setModel(modelStatus);
    //     ui->tvwStatus->resizeColumnsToContents();
    // }
}

void wProjectManagement::on_actionOpen_triggered()
{
    selectProjectFolder();
}

void wProjectManagement::on_btnStatusGetStatus_clicked()
{
    getGitStatus();
}

void wProjectManagement::on_btnDe_selectAll_clicked()
{
    if (isAllSelected)
    {
        for (int i = 0; i < ui->twgStatus->rowCount(); i++) ui->twgStatus->item(i, 0)->setCheckState(Qt::Unchecked);
        isAllSelected = false;
    }
    else
    {
        for (int i = 0; i < ui->twgStatus->rowCount(); i++) ui->twgStatus->item(i, 0)->setCheckState(Qt::Checked);
        isAllSelected = true;
    }
}

void wProjectManagement::on_twgStatus_itemClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    isAllSelected = false;
}

void wProjectManagement::on_twgStatus_itemDoubleClicked(QTableWidgetItem *item)
{
    QString filename = ui->twgStatus->item(item->row(), 1)->text();
    if (filename.contains(">")) filename = filename.mid(filename.indexOf(">") + 2);

    QString diff = git.exec(QStringList() << "diff" << filename).first;

    if (diff.isEmpty()) diff = "(no file comparison possible - no changes, or file was deleted)";
    else if (diff.contains("Binary")) diff = "Binary files - detected file changes";

    ui->pteOutputInfo->setPlainText(diff);
}

void wProjectManagement::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPREFERENCES->show();
}

void wProjectManagement::on_actionProjectSettings_triggered()
{
    WPROJECTPREFERENCES = new wProjectPreferences(git.projectFolder);
    WPROJECTPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPROJECTPREFERENCES->show();
}

void wProjectManagement::on_actionNewProject_triggered()
{
    WCREATEPROJECT = new wCreateProject();
    WCREATEPROJECT->setWindowModality(Qt::ApplicationModal);
    WCREATEPROJECT->show();

    QObject::connect(WCREATEPROJECT, &wCreateProject::creationFinished, this, &wProjectManagement::recieveNewProject);
}

void wProjectManagement::on_btnTest_clicked()
{
    ui->pteOutputInfo->setPlainText(git.exec(QStringList() << "status").first);
}

void wProjectManagement::reloadDownloadedProject(QString path)
{
    qInfo() << "Downloaded git - path:" << path;
    git.projectFolder = path;
    loadProject();
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Save changes in project"), tr("To save changes in this project, you need to input a name and an e-mail adress. Input identity now?"));

    if (reply == QMessageBox::Yes) on_actionProjectSettings_triggered();
}

void wProjectManagement::recieveNewProject(QString path)
{
    git.projectFolder = path;
    loadProject();
}

void wProjectManagement::on_btnManageFiles_clicked()
{
    WMANAGEFILES = new wManageFiles();
    WMANAGEFILES->setWindowModality(Qt::ApplicationModal);
    WMANAGEFILES->show();
}

void wProjectManagement::on_actionDownloadProject_triggered()
{
    if (git.projectFolder != "") WDOWNLOADPROJECT = new wDownloadProject(git.exec(QStringList() << "remote" << "get-url" << "origin").first.trimmed(), git.projectFolder);
    else WDOWNLOADPROJECT = new wDownloadProject();

    WDOWNLOADPROJECT->setWindowModality(Qt::ApplicationModal);
    WDOWNLOADPROJECT->show();

    QObject::connect(WDOWNLOADPROJECT, &wDownloadProject::downloadFinished, this, &wProjectManagement::reloadDownloadedProject);
}

void wProjectManagement::on_actionUploadProject_triggered()
{

}

void wProjectManagement::on_actionCloseProject_triggered()
{
    clearUi();
    enableUi(false);
    git.projectFolder = "";
}

