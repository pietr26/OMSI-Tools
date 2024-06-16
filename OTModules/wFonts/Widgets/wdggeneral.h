#ifndef WDGGENERAL_H
#define WDGGENERAL_H

#include <QWidget>
#include "OTBackend/OTOmsiFileHandler.h"

namespace Ui {
class wdgGeneral;
}

class wdgGeneral : public QWidget
{
    Q_OBJECT

public:
    explicit wdgGeneral(QWidget *parent, OTFontModel *font);
    ~wdgGeneral();

private slots:
    void on_ledFontName_textChanged(const QString &arg1);

    void on_ledColorTexture_textChanged(const QString &arg1);

    void on_ledAlphaTexture_textChanged(const QString &arg1);

    void on_btnColorTexture_clicked();

    void on_btnAlphaTexture_clicked();

    void on_sbxMaxHeigthOfChars_textChanged(const QString &arg1);

    void on_sbxDistanceBetweenChars_textChanged(const QString &arg1);

public slots:
    void reloadUi();

signals:
    void setModified(bool state);

    void checkCharValidity();

    void reloadPreview();

private:
    Ui::wdgGeneral *ui;

    OTSettings set;

    OTFontModel *_font;

    void checkPropValidity();
};

#endif // WDGGENERAL_H
