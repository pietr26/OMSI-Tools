#ifndef WADDFILES_H
#define WADDFILES_H

#include <QMainWindow>

namespace Ui {
class wAddFiles;
}

class wAddFiles : public QMainWindow
{
    Q_OBJECT

public:
    explicit wAddFiles(QWidget *parent = nullptr);
    ~wAddFiles();

signals:
    void submitFiles(QStringList files);

private slots:
    void on_btnAdd_clicked();

private:
    Ui::wAddFiles *ui;
};

#endif // WADDFILES_H
