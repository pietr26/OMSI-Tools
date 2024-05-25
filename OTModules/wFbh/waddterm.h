#ifndef WADDTERM_H
#define WADDTERM_H

#include <QMainWindow>

namespace Ui {
class wAddTerm;
}

class wAddTerm : public QMainWindow
{
    Q_OBJECT

public:
    explicit wAddTerm(bool isNewEntry, QWidget *parent = nullptr, int ID = -1, QString argument = QString());
    ~wAddTerm();

private slots:
    void on_btnSave_clicked();

    void on_pteArgument_textChanged();

signals:
    void dialogFinished(int ID, QString argument);

private:
    Ui::wAddTerm *ui;
};

#endif // WADDTERM_H
