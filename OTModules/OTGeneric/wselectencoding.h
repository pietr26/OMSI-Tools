#ifndef WSELECTENCODING_H
#define WSELECTENCODING_H

#include <QDialog>
#include <QStringConverter>

namespace Ui {
class wSelectEncoding;
}

/*!
    Asks which encoding a font file should be read with.

    Used through exec(): modal by construction, lives on the caller's stack and hands
    its result back through selectedEncoding(). It used to be a QMainWindow which was
    shown non-modally, reported through a signal and was never freed.
*/
class wSelectEncoding : public QDialog
{
    Q_OBJECT

public:
    explicit wSelectEncoding(QWidget *parent = nullptr);
    ~wSelectEncoding();

    /// Only meaningful after exec() returned Accepted.
    QStringConverter::Encoding selectedEncoding() const;

private slots:
    void on_btnSelect_clicked();

private:
    Ui::wSelectEncoding *ui;
};

#endif // WSELECTENCODING_H
