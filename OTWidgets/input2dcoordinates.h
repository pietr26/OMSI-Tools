#ifndef INPUT2DCOORDINATES_H
#define INPUT2DCOORDINATES_H

#include <QWidget>

namespace Ui {
class input2DCoordinates;
}

class input2DCoordinates : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString suffix WRITE setSuffix READ getSuffix)
    Q_PROPERTY(int decimals WRITE setDecimals READ getDecimals)
    Q_PROPERTY(double min WRITE setMin READ getMin)
    Q_PROPERTY(double max WRITE setMax READ getMax)
    Q_PROPERTY(QString name1 WRITE setName1 READ getName1)
    Q_PROPERTY(QString name2 WRITE setName2 READ getName2)

public:
    explicit input2DCoordinates(QWidget *parent = nullptr);
    ~input2DCoordinates();

    float getValue1();
    void setValue1(float value);

    float getValue2();
    void setValue2(float value);

signals:
    void name1Changed(float value);
    void name2Changed(float value);

private slots:
    void on_sbx1_valueChanged(double arg1);

    void on_sbx2_valueChanged(double arg1);

private:
    Ui::input2DCoordinates *ui;

    QString name1;
    QString name2;
    QString name3;

    void setSuffix(QString suffix);
    void setDecimals(int decimals);
    void setMin(double min);
    void setMax(double max);
    void setName1(QString name1);
    void setName2(QString name2);

    QString getSuffix();
    int getDecimals();
    double getMin();
    double getMax();
    QString getName1();
    QString getName2();
};

#endif // INPUT2DCOORDINATES_H
