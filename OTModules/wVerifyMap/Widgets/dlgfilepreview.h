#ifndef DLGFILEPREVIEW_H
#define DLGFILEPREVIEW_H

#include <QDialog>
#include "OTBackend/OTContentValidator/OTContentValidator.h"
#include <QListWidgetItem>

namespace Ui {
class dlgFilePreview;
}

class dlgFilePreview : public QDialog
{
    Q_OBJECT

public:
    explicit dlgFilePreview(QWidget *parent, const QString &filePath, const OTContentValidatorResult &result);
    ~dlgFilePreview();

protected:
    void loadFile();

private slots:
    void on_btnOpenFile_clicked();

private:
    Ui::dlgFilePreview *ui;

    QString _filePath;
    OTContentValidatorResult _result;
};

#endif // DLGFILEPREVIEW_H
