#include "wScriptEditor.h"
#include "ui_wScriptEditor.h"

wScriptEditor::wScriptEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wScriptEditor) {
    ui->setupUi(this);
}

wScriptEditor::~wScriptEditor() {
    delete ui;
}

void wScriptEditor::on_actionBackToHome_triggered() {
    if(close())
        emit backToHome();
}

void wScriptEditor::on_actionClose_triggered() {
    close();
}
