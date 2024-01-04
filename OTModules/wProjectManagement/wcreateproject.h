#ifndef WCREATEPROJECT_H
#define WCREATEPROJECT_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include <QRegularExpression>
#include <QValidator>

namespace Ui {
class wCreateProject;
}

class wCreateProject : public QMainWindow
{
    Q_OBJECT

public:
    explicit wCreateProject(QWidget *parent = nullptr);
    ~wCreateProject();

signals:
    void creationFinished(QString path);

private slots:
    void on_btnSelectFolder_clicked();

    void on_btnCreate_clicked();

    void on_btnCancel_clicked();

    void on_ledName_textChanged(const QString &arg1);

    void on_ledFolder_textChanged(const QString &arg1);

private:
    Ui::wCreateProject *ui;

    OTSettings set;
    OTGit git;

    bool verifyData();

    QValidator *nameValidator;
};

#endif // WCREATEPROJECT_H
