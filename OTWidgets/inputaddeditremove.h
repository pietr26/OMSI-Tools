#ifndef INPUTADDEDITREMOVE_H
#define INPUTADDEDITREMOVE_H

#include <QWidget>

namespace Ui {
class inputAddEditRemove;
}

class inputAddEditRemove : public QWidget
{
    Q_OBJECT

public:
    explicit inputAddEditRemove(QWidget *parent = nullptr);
    ~inputAddEditRemove();

    void setAddEnabled(bool enabled);

    void setEditEnabled(bool enabled);

    void setRemoveEnabled(bool enabled);

signals:
    void addClicked();

    void editClicked();

    void removeClicked();

private slots:
    void on_btnAdd_clicked();

    void on_btnEdit_clicked();

    void on_btnRemove_clicked();

private:
    Ui::inputAddEditRemove *ui;
};

#endif // INPUTADDEDITREMOVE_H
