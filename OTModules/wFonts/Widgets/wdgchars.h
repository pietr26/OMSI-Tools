#ifndef WDGCHARS_H
#define WDGCHARS_H

#include <QWidget>
#include "OTBackend/OTOmsiFileHandler.h"
#include "OTModules/OTGeneric/wpreferences.h"

namespace Ui {
class wdgChars;
}

class wdgChars : public QWidget
{
    Q_OBJECT

public:
    explicit wdgChars(QWidget *parent, OTFontModel *font);
    ~wdgChars();

private slots:
    void on_btnNewChar_clicked();

    void on_btnDeleteSelection_clicked();

    void on_btnMoveUp_clicked();

    void on_btnMoveDown_clicked();

    void on_ledCharacter_textChanged(const QString &arg1);

    void on_sbxLeftPixel_textChanged(const QString &arg1);

    void on_sbxRightPixel_textChanged(const QString &arg1);

    void on_sbxHighestPixelInFontRow_textChanged(const QString &arg1);

    void on_ledComment_textChanged(const QString &arg1);

    void on_btnFind_clicked();

    void on_btnNextResult_clicked();

    void on_btnEditorPreferences_clicked();

    void on_lvwChars_pressed(const QModelIndex &index);

public slots:
    void reloadUi();

    void checkCharValidity();

    void switchCurrentChar();

signals:
    void setModified(bool state);

private:
    Ui::wdgChars *ui;

    OTSettings set;
    OTMessage msg;

    OTFontModel *_font;

    wPreferences *WPREFERENCES = new wPreferences(this, "wFonts");

    void newChar();

    bool charListUpdate = false;

    bool charUIUpdate = false;

    QStringListModel *strListChars = new QStringListModel();

    void clear(bool onlyChar);

    void moveChar(int selection, QString action);

    QString currentSearch;

    void charSelectionChanged(const QModelIndex &newSel, const QModelIndex &oldSel);
};

#endif // WDGCHARS_H
