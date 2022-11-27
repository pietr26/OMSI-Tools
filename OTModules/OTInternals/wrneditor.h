#ifndef WRNEDITOR_H
#define WRNEDITOR_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include <QJsonDocument>

namespace Ui {
class wRNEditor;
}

class wRNEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit wRNEditor(QWidget *parent = nullptr);
    ~wRNEditor();

private slots:
    void on_btnLoad_clicked();

    void on_btnSave_clicked();

    void on_btnAddSelection_clicked();

    void on_btnRemoveSelection_clicked();

    void on_btnAddOthers_clicked();

    void on_btnRemoveOthers_clicked();

private:
    Ui::wRNEditor *ui;

    OTSettings set;
    OTMiscellaneous misc;
};

#endif // WRNEDITOR_H
