#ifndef WSTYLETEST_H
#define WSTYLETEST_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include <QStandardItemModel>

namespace Ui {
class wStyleTest;
}

class wStyleTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit wStyleTest(QWidget *parent = nullptr);
    ~wStyleTest();

private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    const QString moduleName = "wStyleTest";
    Ui::wStyleTest *ui;
    OTSettings set;
};

#endif // WSTYLETEST_H
