#ifndef WDGGENERAL_H
#define WDGGENERAL_H

#include <QWidget>
#include "OTBackend/OCC.h"

namespace Ui {
class wdgGeneral;
}

class wdgGeneral : public QWidget
{
    Q_OBJECT

public:
    explicit wdgGeneral(QWidget *parent, OCFont *font);
    ~wdgGeneral();

private slots:
    void on_ledFontName_textChanged(const QString &arg1);

    void on_ledColorTexture_textChanged(const QString &arg1);

    void on_ledAlphaTexture_textChanged(const QString &arg1);

    void on_btnColorTexture_clicked();

    void on_btnAlphaTexture_clicked();

    void on_sbxMaxHeigthOfChars_valueChanged(int arg1);

    void on_sbxDistanceBetweenChars_valueChanged(int arg1);

    void on_cobxCurrentFont_currentIndexChanged(int index);

public slots:
    void reloadUi();

    void changeFontIndex(int index);

signals:
    void setModified(bool state);

    void checkCharValidity();

    void reloadPreview();

    void fontIndexChanged(int index);

private:
    Ui::wdgGeneral *ui;

    OTSettings set;

    OCFont *_font;

    int currentFontIndex = 0;

    void checkPropValidity();

    bool setupFinished = false;
};

#endif // WDGGENERAL_H
