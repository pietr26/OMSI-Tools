#ifndef WPROJECTMANAGEMENT_H
#define WPROJECTMANAGEMENT_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wProjectManagement;
}

class wProjectManagement : public QMainWindow
{
    Q_OBJECT

public:
    explicit wProjectManagement(QWidget *parent = nullptr);
    ~wProjectManagement();

private slots:
    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

    void on_btnPullDownload_clicked();

    void on_btnFetchDownload_clicked();

    void on_btnCommitSave_clicked();

    void on_btnPushUpload_clicked();

signals:
    void backToHome();

private:
    Ui::wProjectManagement *ui;
    OTSettings set;

    void execGit(QStringList args);
};

#endif // WPROJECTMANAGEMENT_H
