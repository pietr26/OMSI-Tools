#ifndef WADDWORD_H
#define WADDWORD_H

#include <QMainWindow>

namespace Ui {
class wAddWord;
}

class wAddWord : public QMainWindow
{
    Q_OBJECT

public:
    explicit wAddWord(bool isNewEntry, QWidget *parent = nullptr, int ID = -1, QString word = "");
    ~wAddWord();

signals:
    void dialogFinished(int ID, QString word);

private slots:
    void on_btnSave_clicked();

    void on_ledWord_textChanged(const QString &arg1);

private:
    Ui::wAddWord *ui;
};

#endif // WADDWORD_H
