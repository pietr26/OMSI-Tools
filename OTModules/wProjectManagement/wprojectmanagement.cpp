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
    git.exec(QStringList() << "XXX");
}

void wProjectManagement::on_btnPushUpload_clicked()
{
    git.exec(QStringList() << "XXX");
}

void wProjectManagement::selectProjectFolder()
{
    git.projectFolder.clear();
    git.projectFolder = QFileDialog::getExistingDirectory(this, tr("Select project folder"), set.read(objectName(), "lastProjectFolder").toString().isEmpty() ? set.read("main", "mainDir").toString() : set.read(objectName(), "lastProjectFolder").toString());

    if (!git.projectFolder.isEmpty())
    {
        set.write(objectName(), "lastProjectFolder", git.projectFolder);

        getGitStatus();

        watcher = new QFileSystemWatcher(QStringList() << git.projectFolder);
        QObject::connect(watcher, &QFileSystemWatcher::fileChanged, this, &wProjectManagement::restartFileWatcherTimer);
        QObject::connect(watcher, &QFileSystemWatcher::directoryChanged, this, &wProjectManagement::restartFileWatcherTimer);

        enableUi(true);
    }
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
        else                   status << QPair<QString, QString>("???",                         output[i].remove(0, 3));
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
    qInfo() << "Diff";
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
