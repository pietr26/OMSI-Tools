#ifndef WFEEDBACK_H
#define WFEEDBACK_H

#include <QMainWindow>

namespace Ui {
class wFeedback;
}

class wFeedback : public QMainWindow
{
    Q_OBJECT

public:
    explicit wFeedback(QWidget *parent = nullptr);
    ~wFeedback();

private:
    Ui::wFeedback *ui;
};

#endif // WFEEDBACK_H
