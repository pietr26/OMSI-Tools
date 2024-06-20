#ifndef WDGCHARS_H
#define WDGCHARS_H

#include <QWidget>
#include "OTBackend/OCC.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include <QScrollBar>

namespace Ui {
class wdgChars;
}

class wdgChars : public QWidget

{
    Q_OBJECT

public:
    explicit wdgChars(QWidget *parent, OCFont *font);
    ~wdgChars();

private slots:
    void on_btnNewChar_clicked();

    void on_btnNewFont_clicked();

    void on_btnDeleteSelection_clicked();

    void on_btnMoveUp_clicked();

    void on_btnMoveDown_clicked();

    void on_ledCharacter_textChanged(const QString &arg1);

    void on_sbxLeftPixel_valueChanged(int arg1);

    void on_sbxRightPixel_valueChanged(int arg1);

    void on_sbxHighestPixelInFontRow_valueChanged(int arg1);

    void on_btnFind_clicked();

    void on_btnNextResult_clicked();

    void on_btnEditorPreferences_clicked();

    void on_btnTest_clicked();

    void on_ledFontName_textChanged(const QString &arg1);

    void on_ledColorTexture_textChanged(const QString &arg1);

    void on_btnColorTexture_clicked();

    void on_ledAlphaTexture_textChanged(const QString &arg1);

    void on_btnAlphaTexture_clicked();

    void on_sbxMaxHeigthOfChars_valueChanged(int arg1);

    void on_sbxDistanceBetweenChars_valueChanged(int arg1);

public slots:
    void reloadUi();

    void checkCharValidity(); // TODO: combine?
    void checkPropValidity(); // |_

    void switchSelection();

signals:
    void setModified(bool state);

    void reloadActionStates();

    void reloadPreview();

private:
    Ui::wdgChars *ui;

    OTSettings set;
    OTMessage msg;

    OCFont *_font;

    wPreferences *WPREFERENCES = new wPreferences(this, "wFonts");

    QStandardItemModel *model = new QStandardItemModel();

    void clear(bool onlyChar);

    void moveChar(int selection, QString action);

    QString currentSearch;
};

#endif // WDGCHARS_H
