#ifndef DLGSOURCESLIST_H
#define DLGSOURCESLIST_H

#include <QDialog>

namespace Ui {
class dlgSourcesList;
}

class dlgSourcesList : public QDialog
{
    Q_OBJECT

public:
    explicit dlgSourcesList(QWidget *parent = nullptr);
    ~dlgSourcesList();

    void addSources(const QList<QPair<QString, int>> &);

private slots:
    void onCopySelected();
    void on_btnCopyAll_clicked();

private:
    Ui::dlgSourcesList *ui;

    void copyListToClipboard(const QStringList &);
};

#endif // DLGSOURCESLIST_H
