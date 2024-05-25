#ifndef WSELECTENCODING_H
#define WSELECTENCODING_H

#include <QMainWindow>

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
};

#endif // WSELECTENCODING_H
