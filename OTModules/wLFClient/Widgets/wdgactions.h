#ifndef WDGACTIONS_H
#define WDGACTIONS_H

#include <QWidget>

namespace Ui {
class wdgActions;
}

class wdgActions : public QWidget
{
    Q_OBJECT

public:
    explicit wdgActions(QWidget *parent = nullptr);
    ~wdgActions();

private slots:
    void on_btnSpeak_clicked();

    void on_btnCrash_clicked();

private:
    Ui::wdgActions *ui;

    void sendStatus();
};

#endif // WDGACTIONS_H
