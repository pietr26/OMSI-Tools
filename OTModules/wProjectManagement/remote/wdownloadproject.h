#ifndef WDOWNLOADPROJECT_H
#define WDOWNLOADPROJECT_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTExternal.h"

namespace Ui {
class wDownloadProject;
}

class wDownloadProject : public QMainWindow
{
    Q_OBJECT

public:
    explicit wDownloadProject(QString gitPath = "", QString projectPath = "", QWidget *parent = nullptr);
    ~wDownloadProject();

signals:
    void downloadFinished(QString path);

private slots:
    void on_btnCancel_clicked();

    void on_btnDownload_clicked();

    void on_btnSelectProjectPath_clicked();

    void on_ledProjectPath_textChanged(const QString &arg1);

    void on_ledGitURL_textChanged(const QString &arg1);

private:
    Ui::wDownloadProject *ui;

    OTSettings set;
    OTGit git;

    bool doPull = false;
};

#endif // WDOWNLOADPROJECT_H
