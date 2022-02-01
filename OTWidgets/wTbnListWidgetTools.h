#ifndef WTBNLISTWIDGETTOOLS_H
#define WTBNLISTWIDGETTOOLS_H

#include <QWidget>

namespace Ui {
class wdLwgTools;
}

class wdLwgTools : public QWidget
{
    Q_OBJECT

public:
    explicit wdLwgTools(QWidget *parent = nullptr);
    ~wdLwgTools();

private:
    Ui::wdLwgTools *ui;
};

#endif // WTBNLISTWIDGETTOOLS_H
