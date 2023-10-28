#ifndef WDBKNOWNWORDS_H
#define WDBKNOWNWORDS_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTDatabaseHandler.h"
#include "waddword.h"

namespace Ui {
class wDBKnownWords;
}

class wDBKnownWords : public QMainWindow
{
    Q_OBJECT

public:
    explicit wDBKnownWords(QWidget *parent = nullptr);
    ~wDBKnownWords();

private slots:
    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

signals:
    void backToHome();

private:
    Ui::wDBKnownWords *ui;
    OTSettings set;
    OTDatabaseHandler dbHandler;

    void updateView();

    void addWordFinished(int ID, QString word);
};

#endif // WDBKNOWNWORDS_H
