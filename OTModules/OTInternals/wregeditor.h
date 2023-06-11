#ifndef WREGEDITOR_H
#define WREGEDITOR_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wRegEditor;
}

class wRegEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit wRegEditor(QWidget *parent = nullptr);
    ~wRegEditor();

signals:
    void backToHome();

private slots:
    void on_twgRegEditor_cellChanged(int row, int column);

    void on_btnReload_clicked();

    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

private:
    Ui::wRegEditor *ui;
    OTSettings set;
    OTMiscellaneous misc;

    void loadRegs();
    bool loadRegsFinished = false;
};

#endif // WREGEDITOR_H
