#ifndef WBUGDOC_H
#define WBUGDOC_H

#include <QMainWindow>

namespace Ui {
class wBugDoc;
}

class wBugDoc : public QMainWindow
{
    Q_OBJECT

public:
    explicit wBugDoc(QWidget *parent = nullptr);
    ~wBugDoc();

private:
    Ui::wBugDoc *ui;
};

#endif // WBUGDOC_H
