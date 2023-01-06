#ifndef WSELECTENCODING_H
#define WSELECTENCODING_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wSelectEncoding;
}

class wSelectEncoding : public QMainWindow
{
    Q_OBJECT

public:
    explicit wSelectEncoding(QWidget *parent = nullptr);
    ~wSelectEncoding();

signals:
    void encodingSelected(QStringConverter::Encoding encoding);

private slots:
    void on_btnSelect_clicked();

private:
    Ui::wSelectEncoding *ui;

    OTSettings set;
};

#endif // WSELECTENCODING_H
