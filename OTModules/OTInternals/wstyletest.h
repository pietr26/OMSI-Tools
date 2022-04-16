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

    void loadStyleSheet();

    void on_toolButton_clicked();

    void on_btnSetThemeNone_clicked();

    void on_btnSetThemeCombinear_clicked();

    void on_btnSetThemeDarkeum_clicked();

private:
    const QString moduleName = "wStyleTest";
    Ui::wStyleTest *ui;
    OTSettings set;
    OTMiscellaneous misc;
    QTimer *timer = new QTimer(this);
};

#endif // WSTYLETEST_H
