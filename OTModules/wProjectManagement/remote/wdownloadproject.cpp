#include "wdownloadproject.h"
#include "ui_wdownloadproject.h"

wDownloadProject::wDownloadProject(QString gitPath, QString projectPath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wDownloadProject)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    if (!gitPath.isEmpty()) ui->ledGitURL->setEnabled(false);
    if (!projectPath.isEmpty())
    {
        ui->ledProjectPath->setEnabled(false);
        ui->btnSelectProjectPath->setEnabled(false);
    }

    if (!gitPath.isEmpty() || !projectPath.isEmpty())
    {
        doPull = true;
        ui->lblProjectFolderInfo->setVisible(false);
    }
    else ui->lblResetInfo->setVisible(false);

    on_ledProjectPath_textChanged("");

    ui->ledProjectPath->setText(projectPath);
    ui->ledGitURL->setText(gitPath);

    // Load prefs
    setWindowTitle(tr("Download project"));

    qInfo().noquote() << objectName() + " started";
}

wDownloadProject::~wDownloadProject()
{
    delete ui;
}

void wDownloadProject::on_btnCancel_clicked()
{
    close();
}

void wDownloadProject::on_btnDownload_clicked()
{
    git.projectFolder = ui->ledProjectPath->text();

    if (doPull)
    {
        QMessageBox::StandardButton reply = QMessageBox::information(this, tr("Overwrite local files"), tr("By downloading the project, changed local files might will be overwritten. Resume?"), QMessageBox::Ok | QMessageBox::Cancel);

        if (reply == QMessageBox::Ok)
        {
            git.exec(QStringList() << "pull" << ui->ledGitURL->text());
            emit downloadFinished(ui->ledProjectPath->text());
        }
    }
    else
    {
        QPair<QString, QString> execResult = git.exec(QStringList() << "clone" << ui->ledGitURL->text());
        QStringList info = execResult.second.split("\n");

        if (execResult.second.contains("not found"))
        {
            QMessageBox::critical(this, tr("Error while downloading project"), tr("The given URL does not exist or is not a repository."));
            qCritical().noquote().nospace() << "Cannot download git repo '" << ui->ledGitURL->text() << "' to '" << ui->ledProjectPath << "': Given URL does not exist or is not a repository:" << execResult.second;
            return;
        }
        else if (execResult.second.contains("unable to access"))
        {
            QMessageBox::critical(this, tr("Error while downloading project"), tr("Cannot access to the given URL."));
            qCritical().noquote().nospace() << "Cannot download git repo '" << ui->ledGitURL->text() << "' to '" << ui->ledProjectPath << "': Cannot access to the given URL";
            return;
        }
        else if (execResult.second.contains("aleady exists and is not an empty directory"))
        {
            // TODO: Remove this elseif and integrate realtime check (on_bla_changed -> check if folder already exists)
            QMessageBox::critical(this, tr("Error while downloading project"), tr(""));
            qCritical().noquote().nospace() << "Cannot download git repo '" << ui->ledGitURL->text() << "' to '" << ui->ledProjectPath << "': Cannot access to the given URL";
            return;
        }
        else if (execResult.second.contains("fatal"))
        {
            QMessageBox::critical(this, tr("Error while downloading project"), tr("Unknown fatal error:") + "\n" + execResult.second);
            qCritical().noquote().nospace() << "Cannot download git repo '" << ui->ledGitURL->text() << "' to '" << ui->ledProjectPath << "' - fatal error: " << execResult.second;
            return;
        }

        emit downloadFinished(ui->ledProjectPath->text() + "/" + info[0].replace("Cloning into '", "").replace("'...", ""));
    }

    close();
}

void wDownloadProject::on_btnSelectProjectPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Select destination"));
    ui->ledProjectPath->setText(path);
}

void wDownloadProject::on_ledProjectPath_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->btnDownload->setEnabled(QDir(ui->ledProjectPath->text()).exists() && ui->ledGitURL->text().startsWith("https://") && !ui->ledProjectPath->text().isEmpty() && !ui->ledGitURL->text().isEmpty());
}

void wDownloadProject::on_ledGitURL_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->btnDownload->setEnabled(QDir(ui->ledProjectPath->text()).exists() && ui->ledGitURL->text().startsWith("https://") && !ui->ledProjectPath->text().isEmpty() && !ui->ledGitURL->text().isEmpty());
}
