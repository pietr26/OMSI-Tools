#ifndef WSCRIPTEDITOR_H
#define WSCRIPTEDITOR_H

#include <QMainWindow>

namespace Ui {
class wScriptEditor;
}

class wScriptEditor : public QMainWindow {
    Q_OBJECT

public:
    explicit wScriptEditor(QWidget *parent = nullptr);
    ~wScriptEditor();

signals:
    void backToHome();

private slots:
    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

private:
    Ui::wScriptEditor *ui;
};

#endif // WSCRIPTEDITOR_H
