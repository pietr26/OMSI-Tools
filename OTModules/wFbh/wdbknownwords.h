#ifndef WDBKNOWNWORDS_H
#define WDBKNOWNWORDS_H

#include <QMainWindow>
#include "OTBackend/OTExternal.h"
#include "OTBackend/OTGlobal.h"
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

    void on_tvwWords_activated(const QModelIndex &index);

signals:
    void backToHome();

private:
    Ui::wDBKnownWords *ui;
    OTDatabaseHandler dbHandler;
    OTMiscellaneous misc;

    void updateView();

    void addWordFinished(int ID, QString word);
};

#endif // WDBKNOWNWORDS_H
