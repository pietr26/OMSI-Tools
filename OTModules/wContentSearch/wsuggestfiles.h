#ifndef WSUGGESTFILES_H
#define WSUGGESTFILES_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wSuggestFiles;
}

class wSuggestFiles : public QMainWindow
{
    Q_OBJECT

public:
    explicit wSuggestFiles(QWidget *parent = nullptr, QString links = "");
    ~wSuggestFiles();

private slots:
    void on_btnRules_clicked();

    void on_btnSend_clicked();

    void on_pteFiles_textChanged();

    void on_cbxAcceptRules_stateChanged(int arg1);

private:
    Ui::wSuggestFiles *ui;

    OTSettings set;
    OTMiscellaneous misc;
    OTNetworkConnection nc;

    bool isReport = false;
};

#endif // WSUGGESTFILES_H
