#ifndef WUPLOADPROJECT_H
#define WUPLOADPROJECT_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wUploadProject;
}

class wUploadProject : public QMainWindow
{
    Q_OBJECT

public:
    explicit wUploadProject(QWidget *parent = nullptr);
    ~wUploadProject();

private slots:
    void on_btnCancel_clicked();

private:
    Ui::wUploadProject *ui;

    OTSettings set;
    OTGit git;
};

#endif // WUPLOADPROJECT_H
