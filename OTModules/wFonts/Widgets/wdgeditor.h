#ifndef WDGEDITOR_H
#define WDGEDITOR_H

#include <QWidget>
#include "OTBackend/OCC.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include <QScrollBar>

namespace Ui {
class wdgEditor;
}

class wdgEditor : public QWidget

{
    Q_OBJECT

public:
    explicit wdgEditor(QWidget *parent, OCFont *font);
    ~wdgEditor();

    QList<QAction*> actionsEdit;

    void unexpandAll();

private slots:
    void on_ledCharacter_textChanged(const QString &arg1);

    void on_sbxLeftPixel_valueChanged(int arg1);

    void on_sbxRightPixel_valueChanged(int arg1);

    void on_sbxHighestPixelInFontRow_valueChanged(int arg1);

    void on_btnEditorPreferences_clicked();

    void on_ledFontName_textChanged(const QString &arg1);

    void on_ledColorTexture_textChanged(const QString &arg1);

    void on_btnColorTexture_clicked();

    void on_ledAlphaTexture_textChanged(const QString &arg1);

    void on_btnAlphaTexture_clicked();

    void on_sbxMaxHeigthOfChars_valueChanged(int arg1);

    void on_sbxDistanceBetweenChars_valueChanged(int arg1);

    void on_btnCloseSearch_clicked();

public slots:
    void reloadUi();

    void checkCharValidity(); // TODO: combine?
    void checkPropValidity(); // |_

    void switchSelection();

    void addFont();
    void addCharacter();
    void deleteItem();
    void search();
    void goToNextError();

signals:
    void setModified(bool state);

    void reloadActionStates();

    void reloadPreview();

private:
    Ui::wdgEditor *ui;

    OTSettings set;
    OTMessage msg;

    OCFont *_font;

    enum Move
    {
        Up,
        Down
    };

    QList<bool> fontExpansions;

    wPreferences *WPREFERENCES = new wPreferences(this, "wFonts");

    QStandardItemModel *model = new QStandardItemModel();

    void clear(bool onlyChar);

    void moveChar(int selection, Move action);

    QString currentSearch;

    QAction *actionAddFont;
    QAction *actionAddCharacter;
    QAction *actionDeleteItem;
    QAction *actionMoveUp;
    QAction *actionMoveDown;
    QAction *actionSearch;
    QAction *actionGoToNextError;
};

#endif // WDGEDITOR_H
