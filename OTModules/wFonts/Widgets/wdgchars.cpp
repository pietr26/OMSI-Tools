#include "wdgchars.h"
#include "ui_wdgchars.h"

wdgChars::wdgChars(QWidget *parent, OTFontModel *font)
    : QWidget(parent)
    , ui(new Ui::wdgChars),
    _font(font)
{
    ui->setupUi(this);

    ui->sbxHighestPixelInFontRow->clear();
    ui->sbxLeftPixel->clear();
    ui->sbxRightPixel->clear();

    ui->btnNextResult->setEnabled(false);
    ui->btnFind->setEnabled(false);

    ui->gbxSearchChar->setVisible(false);

    ui->lvwChars->setModel(strListChars);
    connect(ui->lvwChars->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &wdgChars::charSelectionChanged);

    reloadUi();
}

wdgChars::~wdgChars()
{
    delete ui;
}

void wdgChars::on_btnNewChar_clicked()
{
    newChar();
}

void wdgChars::on_btnDeleteSelection_clicked()
{
    qDebug().noquote() << QString("Delete char '%1' at position %2...").arg(_font->charList.at(ui->lvwChars->currentIndex().row()).character).arg(ui->lvwChars->currentIndex().row());

    if (ui->lvwChars->currentIndex().row() != -1) _font->charList.removeAt(ui->lvwChars->currentIndex().row());

    qDebug() << "Font charlist count:" << _font->charList.count();

    reloadUi();
    switchCurrentChar();

    emit setModified(true);
}

void wdgChars::on_btnMoveUp_clicked()
{
    if (_font->charList.count() != 0) moveChar(ui->lvwChars->currentIndex().row(), "UP");
}

void wdgChars::on_btnMoveDown_clicked()
{
    if (ui->lvwChars->currentIndex().row() != _font->charList.count() - 1) moveChar(ui->lvwChars->currentIndex().row(), "DOWN");
}

void wdgChars::on_ledCharacter_textChanged(const QString &arg1)
{
    if (_font->charList.count() != 0 && ui->lvwChars->currentIndex().row() != -1)
    {
        _font->charList[ui->lvwChars->currentIndex().row()].character = arg1;
        if (!charUIUpdate) reloadUi();
    }

    checkCharValidity();
    emit setModified(true);
}

void wdgChars::on_sbxLeftPixel_textChanged(const QString &arg1)
{
    if (_font->charList.count() != 0 && ui->lvwChars->currentIndex().row() != -1)
    {
        if (arg1.isEmpty()) _font->charList[ui->lvwChars->currentIndex().row()].leftPixel = -1;
        else _font->charList[ui->lvwChars->currentIndex().row()].leftPixel = arg1.toInt();

        if (!charUIUpdate) reloadUi();
    }

    checkCharValidity();
    emit setModified(true);
}


void wdgChars::on_sbxRightPixel_textChanged(const QString &arg1)
{
    if (_font->charList.count() != 0 && ui->lvwChars->currentIndex().row() != -1)
    {
        if (arg1.isEmpty()) _font->charList[ui->lvwChars->currentIndex().row()].rightPixel = -1;
        else _font->charList[ui->lvwChars->currentIndex().row()].rightPixel = arg1.toInt();

        if (!charUIUpdate) reloadUi();
    }

    checkCharValidity();
    emit setModified(true);
}


void wdgChars::on_sbxHighestPixelInFontRow_textChanged(const QString &arg1)
{
    if (_font->charList.count() != 0 && ui->lvwChars->currentIndex().row() != -1)
    {
        if (arg1.isEmpty()) _font->charList[ui->lvwChars->currentIndex().row()].highestPixelInFontRow = -1;
        else _font->charList[ui->lvwChars->currentIndex().row()].highestPixelInFontRow = arg1.toInt();

        if (!charUIUpdate) reloadUi();
    }

    checkCharValidity();
    emit setModified(true);
}

void wdgChars::on_ledComment_textChanged(const QString &arg1)
{
    if (_font->charList.count() != 0 && ui->lvwChars->currentIndex().row() != -1) _font->charList[ui->lvwChars->currentIndex().row()].comment = arg1;

    emit setModified(true);
}

void wdgChars::on_btnFind_clicked()
{
    if (_font->charList.count() == 0)
    {
        msg.noCharsInFont(this);
        return;
    }

    currentSearch = ui->ledSearch->text();

    qDebug().noquote() << "Find char: '" + currentSearch + "'";

    ui->btnNextResult->setEnabled(true);

    // Search for char
    ui->lvwChars->setCurrentIndex(strListChars->index(0));

    for (int i = 0; i < _font->charList.count(); i++)
    {
        if (_font->charList.at(i).character == currentSearch)
        {
            qDebug().noquote() << QString("Char '%1' found at position %2.").arg(currentSearch).arg(i + 1);
            switchCurrentChar();
            return;
        }

        ui->lvwChars->setCurrentIndex(strListChars->index(i + 1));
    }

    // If char couldn't be found
    QMessageBox::information(this, tr("Character not found"), tr("The entered character could not be found."));
    qDebug() << "Character not found.";
    currentSearch = "";
    ui->btnNextResult->setEnabled(false);
}

void wdgChars::on_btnNextResult_clicked()
{
    qDebug() << "Go to next search result";
    if (currentSearch != "")
    {
        if (_font->charList.count() == 1)
            return;

        int i = ui->lvwChars->currentIndex().row();

        QList<OTCharacterModel> tempList;
        tempList = _font->charList;
        tempList.removeAt(ui->lvwChars->currentIndex().row());

        bool secondRound = false;

        if (i >= tempList.count())
        {
            i = 0;
            qDebug() << "Search: End of font, go to top";
            //ui->statusbar->showMessage(tr("The end of the font was reached, search from top"), 4000);
            secondRound = true;
        }

        forever
        {
            if (i > tempList.count() - 1)
            {
                qDebug() << "Search: Not other char found";
                //ui->statusbar->showMessage(tr("No other character found according to the search criteria."), 4000);
                switchCurrentChar();
                return;
            }
            else if (tempList.at(i).character == currentSearch)
            {
                if (i > ui->lvwChars->currentIndex().row() || i == ui->lvwChars->currentIndex().row())
                    i++;
                ui->lvwChars->setCurrentIndex(strListChars->index(i));
                goto end;
            }

            if (!secondRound && (i == tempList.count() - 1))
            {
                i = -1;
                secondRound = true;
            }
            else if (secondRound && (i == ui->lvwChars->currentIndex().row() - 1))
            {
                qDebug() << "Search: Not other char found";
                //ui->statusbar->showMessage(tr("No other character found according to the search criteria."), 4000);
                switchCurrentChar();
                return;
            }

            i++;
        }
    end:
        if (secondRound)
        {
            qDebug() << "Search: End of font, go to top";
            //ui->statusbar->showMessage(tr("The end of the font was reached, search from top"), 4000);
        }

        switchCurrentChar();
    }
}

void wdgChars::on_btnEditorPreferences_clicked()
{
    WPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPREFERENCES->show();
}

void wdgChars::newChar()
{
    qDebug() << "Add new char...";

    int prevPixelRow;
    if(ui->sbxHighestPixelInFontRow->text().isEmpty()) prevPixelRow = -1;
    else prevPixelRow = ui->sbxHighestPixelInFontRow->value();

    _font->charList.insert(ui->lvwChars->currentIndex().row() + 1, OTCharacterModel());

    reloadUi();
    emit setModified(true);

    ui->lvwChars->setCurrentIndex(strListChars->index(ui->lvwChars->currentIndex().row() + 1));

    switchCurrentChar();

    ui->ledCharacter->setFocus();

    if ((set.read(objectName(), "keepPixelRow").toBool()) && (prevPixelRow != -1))
    {
        ui->sbxHighestPixelInFontRow->setValue(prevPixelRow);
        on_sbxHighestPixelInFontRow_textChanged(QString::number(prevPixelRow));
    }
}

void wdgChars::switchCurrentChar()
{
    charUIUpdate = true;
    OTCharacterModel character = _font->charList.isEmpty() ? OTCharacterModel() : _font->charList.at(ui->lvwChars->currentIndex().row());

    ui->ledCharacter->setText(character.character);

    if (character.leftPixel != -1) ui->sbxLeftPixel->setValue(character.leftPixel);
    else ui->sbxLeftPixel->clear();

    if (character.rightPixel != -1) ui->sbxRightPixel->setValue(character.rightPixel);
    else ui->sbxRightPixel->clear();

    if (character.highestPixelInFontRow != -1) ui->sbxHighestPixelInFontRow->setValue(character.highestPixelInFontRow);
    else ui->sbxHighestPixelInFontRow->clear();

    ui->ledComment->setText(character.comment);

    checkCharValidity();
    charUIUpdate = false;
}

void wdgChars::checkCharValidity()
{
    ui->lblCharacter->setStyleSheet("");
    ui->lblRightPixel->setStyleSheet("");
    ui->lblLeftPixel->setStyleSheet("");
    ui->lblHighestPixelInFontRow->setStyleSheet("");

    if (_font->charList.isEmpty() || (ui->lvwChars->currentIndex().row() == -1)) return;

    OTCharacterModel character = _font->charList.at(ui->lvwChars->currentIndex().row());

    if (character.character.isEmpty()) ui->lblCharacter->setStyleSheet("color:red");
    else
    {
        QList<QString> tempCharList;
        foreach (OTCharacterModel current, _font->charList)
            tempCharList << current.character;

        if (tempCharList.indexOf(character.character) != -1)
        {
            tempCharList.removeAt(tempCharList.indexOf(character.character));
            if (tempCharList.indexOf(character.character) != -1)
                ui->lblCharacter->setStyleSheet("color:red");
        }
    }

    if (character.leftPixel > character.rightPixel)
    {
        ui->lblRightPixel->setStyleSheet("color:goldenrod");
        ui->lblLeftPixel->setStyleSheet("color:goldenrod");
    }

    if (character.rightPixel == -1) ui->lblRightPixel->setStyleSheet("color:red");
    if (character.leftPixel == -1) ui->lblLeftPixel->setStyleSheet("color:red");
    if (character.highestPixelInFontRow == -1) ui->lblHighestPixelInFontRow->setStyleSheet("color:red");

    if (QFile(set.read("main", "mainDir").toString() + "/Fonts/" + _font->alphaTexture).exists())
    {
        QImage alphaTexture(set.read("main", "mainDir").toString() + "/Fonts/" + _font->alphaTexture);

        if ((alphaTexture.width() != 0) || (alphaTexture.height() != 0))
        {
            if (character.rightPixel > QString::number(alphaTexture.width()).toInt())
                ui->lblRightPixel->setStyleSheet("color:red");

            if (character.leftPixel > QString::number(alphaTexture.width()).toInt())
                ui->lblLeftPixel->setStyleSheet("color:red");

            if (character.highestPixelInFontRow > QString::number(alphaTexture.width()).toInt())
                ui->lblHighestPixelInFontRow->setStyleSheet("color:red");
        }
    }
}

void wdgChars::moveChar(int selection, QString action) // TODO: make better
{
    if (((selection == 0) && (action == "UP")) ||
        ((selection > _font->charList.count() - 1) && (action == "DOWN")))
        return;

    int moving;

    if (action == "UP") moving = selection - 1;
    else if (action == "DOWN") moving = selection + 1;
    else
    {
        qWarning().noquote() << "Invalid move parameter: " + action;
        return;
    }
    emit setModified(true);

    // Move the selected item down / up
    _font->charList.move(selection, moving);
    reloadUi();
    ui->lvwChars->setCurrentIndex(strListChars->index(moving));
}

void wdgChars::charSelectionChanged(const QModelIndex &newSel, const QModelIndex &oldSel)
{
    Q_UNUSED(newSel); Q_UNUSED(oldSel);

    if (!charListUpdate) on_lvwChars_pressed(QModelIndex());
}

void wdgChars::on_lvwChars_pressed(const QModelIndex &index)
{
    Q_UNUSED(index);

    switchCurrentChar();
    reloadUi();
}

void wdgChars::reloadUi()
{
    /// reload char list
    charListUpdate = true;
    qDebug() << "Updating charlist...";
    int currentRow = ui->lvwChars->currentIndex().row();
    qDebug().noquote() << "currentRow:" << currentRow;

    QStringList chars;

    foreach (OTCharacterModel current, _font->charList)
        chars << current.character;

    strListChars->setStringList(chars);
    ui->lvwChars->setModel(strListChars);

    if (strListChars->index(currentRow).isValid())
        ui->lvwChars->setCurrentIndex(strListChars->index(currentRow));
    else if (strListChars->index(currentRow - 1).isValid())
        ui->lvwChars->setCurrentIndex(strListChars->index(currentRow - 1));
    else
        ui->lvwChars->setCurrentIndex(strListChars->index(-1));

    ui->lblCharCount->setText(QString::number(_font->charList.count()));
    charListUpdate = false;

    checkCharValidity();

    ui->btnDeleteSelection->setDisabled(_font->charList.isEmpty());

    ui->btnMoveUp->setEnabled(ui->lvwChars->currentIndex().row() > 0);
    ui->btnMoveDown->setEnabled(ui->lvwChars->currentIndex().row() < (_font->charList.count() - 1));

    // Reload tex preview here (maybe in thread)
}
