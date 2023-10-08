#ifndef INPUT3DCOORDINATES_H
#define INPUT3DCOORDINATES_H

#include <QWidget>

namespace Ui {
class input3DCoordinates;
}

class input3DCoordinates : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString suffix WRITE setSuffix READ getSuffix)
    Q_PROPERTY(int decimals WRITE setDecimals READ getDecimals)
    Q_PROPERTY(double min WRITE setMin READ getMin)
    Q_PROPERTY(double max WRITE setMax READ getMax)
    Q_PROPERTY(QString name1 WRITE setName1 READ getName1)
    Q_PROPERTY(QString name2 WRITE setName2 READ getName2)
    Q_PROPERTY(QString name3 WRITE setName3 READ getName3)

public:
    explicit input3DCoordinates(QWidget *parent = nullptr);
    ~input3DCoordinates();

    float getValue1();
    void setValue1(float value);

    float getValue2();
    void setValue2(float value);

    float getValue3();
    void setValue3(float value);

signals:
    void name1Changed(float value);
    void name2Changed(float value);
    void name3Changed(float value);

private slots:
    void on_sbx1_valueChanged(double arg1);

    void on_sbx2_valueChanged(double arg1);

    void on_sbx3_valueChanged(double arg1);

private:
    Ui::input3DCoordinates *ui;

    QString name1;
    QString name2;
    QString name3;

    void setSuffix(QString suffix);
    void setDecimals(int decimals);
    void setMin(double min);
    void setMax(double max);
    void setName1(QString name1);
    void setName2(QString name2);
    void setName3(QString name3);

    QString getSuffix();
    int getDecimals();
    double getMin();
    double getMax();
    QString getName1();
    QString getName2();
    QString getName3();
};

#endif // INPUT3DCOORDINATES_H
