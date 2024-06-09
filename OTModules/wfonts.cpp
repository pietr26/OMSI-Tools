#include "wfonts.h"
#include "ui_wfonts.h"


wFonts::wFonts(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wFonts)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("Font creation"));

    DiscordGameSDK::setModule(tr("Font creation"));
    DiscordGameSDK::setStatus(tr("Creating a font"));
    DiscordGameSDK::setStart();
    DiscordGameSDK::setIcon("wfonts");
    DiscordGameSDK::update();

    loadRecentFiles();

    // Set timer for autosave
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(autosave()));
    int autosaveDuration = set.read("main", "autosaveDuration").toInt();
    if (autosaveDuration == 0)
        autosaveDuration = 15;
    timer->start(autosaveDuration * 1000);

    fop.createBackupFolder();

    // Set only-Numbers
    ui->sbxHighestPixelInFontRow->clear();
    ui->sbxLeftPixel->clear();
    ui->sbxRightPixel->clear();
    ui->sbxMaxHeigthOfChars->clear();
    ui->sbxDistanceBetweenChars->clear();

    // and for Debug-Action
    ui->actionGoToNextError->setEnabled(false);

    ui->btnNextResult->setEnabled(false);
    ui->btnFind->setEnabled(false);

    enableFontArea(false);

    loadTexPreview();

    strListChars = new QStringListModel();

    ui->twgFont->setCurrentIndex(0);
    ui->gbxSearchChar->setVisible(false);

    setUnsaved(false);
    ui->lvwChars->setModel(strListChars);
    connect(ui->lvwChars->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &wFonts::charSelectionChanged);
    checkPropValidity();

    // First setup - if not, the application will crash in wFonts::resizeEvent()
    texPreviewScene = new QGraphicsScene(this);
    ui->grvTexPreview->setScene(texPreviewScene);

    setupFinished = true;

    qInfo().noquote() << objectName() + " started";
}

wFonts::~wFonts()
{
    delete ui;
    delete strListChars;
}

void wFonts::closeEvent(QCloseEvent *event)
{
    if (unsaved)
    {
        int msgResult = msg.unsavedChanges(this);
        if (msgResult == -1)
        {
            event->ignore();
            return;
        }
        else if (msgResult == 1)
            save(OTFileMethods::save, font.path);
    }
}

void wFonts::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void wFonts::dropEvent(QDropEvent *e)
{
    QString fileName = e->mimeData()->urls().at(0).toLocalFile();
    qDebug().noquote() << "Dropped file:" << fileName;

    if (QFileInfo(fileName).suffix() != "oft")
    {
        QMessageBox::critical(this, tr("Invalid format"), tr("The dropped file is no font file (*.oft)!"));
        qWarning() << "Dropped file is no OFT file!";
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Open font file"), tr("Open drag and drop font file now?\n%1").arg(fileName));

    if (reply == QMessageBox::Yes)
        open(OTFileMethods::open, fileName);
}

void wFonts::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    resizeTexPreview();
}

void wFonts::on_btnDeleteSelection_clicked()
{
    wFonts::on_actionDeleteSelection_triggered();
}

void wFonts::on_btnNewChar_clicked()
{
    on_actionNewChar_triggered();
}

void wFonts::on_btnMoveUp_clicked()
{
    on_actionMoveCharUp_triggered();
}

void wFonts::on_btnMoveDown_clicked()
{
    on_actionMoveCharDown_triggered();
}

void wFonts::on_actionOpen_triggered()
{
    open(OTFileMethods::open);
}

void wFonts::on_actionReload_triggered()
{
    if (font.path != "")
        open(OTFileMethods::reOpen, font.path);
}

void wFonts::on_actionSave_triggered()
{
    save(OTFileMethods::save, font.path);
}

void wFonts::on_actionSaveAs_triggered()
{
    save(OTFileMethods::saveAs, font.path);
}

void wFonts::enableView(bool status)
{
    centralWidget()->setEnabled(status);
}

void wFonts::loadRecentFiles()
{
    qDebug() << "Read recent files...";
    QStringList recentFiles = set.read(objectName(), "recentFiles").toStringList();

    if (recentFiles.isEmpty())
    {
        ui->menuRecentlyOpenedFonts->setEnabled(false);
        return;
    }
    else
    {
        ui->menuRecentlyOpenedFonts->setEnabled(true);
        ui->menuRecentlyOpenedFonts->clear();
    }

    int i = 1;
    foreach (QString current, recentFiles)
    {
        if (!QFile(current).exists())
            continue;

        QAction *action = new QAction();

        action->setData(current);
        action->setText(QString::number(i) + "      " + QFile(current).fileName());
        action->setVisible(true);
        switch (i)
        {
            case 1: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_1)); break;
            case 2: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_2)); break;
            case 3: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_3)); break;
            case 4: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_4)); break;
            case 5: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_5)); break;
            case 6: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_6)); break;
            case 7: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_7)); break;
            case 8: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_8)); break;
            case 9: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_9)); break;
            case 10: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0)); break;
        }

        connect(action, &QAction::triggered, this, [=]() { this->open(OTFileMethods::open, action->data().toString()); });

        ui->menuRecentlyOpenedFonts->addAction(action);
        i++;
    }

    if (i == 1)
        ui->menuRecentlyOpenedFonts->setEnabled(false);
}

void wFonts::saveRecentFiles(QString absoluteNewFilePath)
{
    qDebug() << "Save recent files...";
    QStringList recentFiles = set.read(objectName(), "recentFiles").toStringList();

    int i = 0;
    foreach (QString current, recentFiles)
    {
        if (!QFile(current).exists())
            recentFiles.removeAt(i);
        else
            i++;
    }

    if (recentFiles.contains(absoluteNewFilePath))
        recentFiles.removeAt(recentFiles.indexOf(absoluteNewFilePath));

    recentFiles.prepend(absoluteNewFilePath);

    while (recentFiles.size() > maxRecentFileCount)
        recentFiles.removeLast();

    set.write(objectName(), "recentFiles", recentFiles);

    loadRecentFiles();
}

void wFonts::setUnsaved(bool state)
{
    if (state)
        unsaved = true;
    else
        unsaved = false;
}

void wFonts::checkPropValidity()
{
    ui->lblFontName->setStyleSheet("");
    ui->lblColorTexture->setStyleSheet("");
    ui->lblAlphaTexture->setStyleSheet("");
    ui->lblMaxHeigthOfChars->setStyleSheet("");
    ui->lblDistanceBetweenChars->setStyleSheet("");

    if (font.name == "")
        ui->lblFontName->setStyleSheet("color:red");

    if ((font.colorTexture != "") && !QFile(set.read("main", "mainDir").toString() + "/Fonts/" + font.colorTexture).exists())
        ui->lblColorTexture->setStyleSheet("color:red");

    if (font.alphaTexture == "" || !QFile(set.read("main", "mainDir").toString() + "/Fonts/" + font.alphaTexture).exists())
        ui->lblAlphaTexture->setStyleSheet("color:red");

    if (ui->sbxMaxHeigthOfChars->text() == "" || (font.maxHeightOfChars == 0))
        ui->lblMaxHeigthOfChars->setStyleSheet("color:red");

    if (ui->sbxDistanceBetweenChars->text() == "")
        ui->lblDistanceBetweenChars->setStyleSheet("color:red");

    if (QFile(set.read("main", "mainDir").toString() + "/Fonts/" + font.alphaTexture).exists())
    {
        QImage alphaTexture(set.read("main", "mainDir").toString() + "/Fonts/" + font.alphaTexture);

        if (alphaTexture.width() != 0 || alphaTexture.height() != 0)
        {
            if (font.maxHeightOfChars > QString::number(alphaTexture.height()).toInt())
                ui->lblMaxHeigthOfChars->setStyleSheet("color:red");

            if (font.distanceBetweenChars > QString::number(alphaTexture.width()).toInt())
                ui->lblDistanceBetweenChars->setStyleSheet("color:red");
        }
    }
}

void wFonts::checkCharValidity()
{
    if (font.charList.size() == 0)
        return;

    ui->lblCharacter->setStyleSheet("");
    ui->lblRightPixel->setStyleSheet("");
    ui->lblLeftPixel->setStyleSheet("");
    ui->lblHighestPixelInFontRow->setStyleSheet("");

    OTCharacterModel character = font.charList.at(ui->lvwChars->currentIndex().row());

    if (character.character == "")
        ui->lblCharacter->setStyleSheet("color:red");
    else
    {
        QList<QString> tempCharList;
        foreach (OTCharacterModel current, font.charList)
            tempCharList << current.character;

        if (tempCharList.indexOf(character.character) != -1)
        {
            tempCharList.removeAt(tempCharList.indexOf(character.character));
            if (tempCharList.indexOf(character.character) != -1)
                ui->lblCharacter->setStyleSheet("color:red");
        }
    }

    if (character.leftPixel >= character.rightPixel)
    {
        ui->lblRightPixel->setStyleSheet("color:goldenrod");
        ui->lblLeftPixel->setStyleSheet("color:goldenrod");
    }

    if (ui->sbxRightPixel->text() == "")
        ui->lblRightPixel->setStyleSheet("color:red");

    if (ui->sbxLeftPixel->text() == "")
        ui->lblLeftPixel->setStyleSheet("color:red");

    if (ui->sbxHighestPixelInFontRow->text() == "")
        ui->lblHighestPixelInFontRow->setStyleSheet("color:red");

    if (QFile(set.read("main", "mainDir").toString() + "/Fonts/" + font.alphaTexture).exists())
    {
        QImage alphaTexture(set.read("main", "mainDir").toString() + "/Fonts/" + font.alphaTexture);

        if (alphaTexture.width() != 0 || alphaTexture.height() != 0)
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

void wFonts::autosave()
{
    if ((set.read("main", "autosave") == "true") && (font.charList.size() != 0))
    {
        qDebug() << "Autosave called";
        save(OTFileMethods::backupSave);
    }
}

void wFonts::selectAllAndClear(bool onlyChar)
{
    if (!onlyChar)
    {
        setUnsaved();
        font.clear();
        strListChars->setStringList(QStringList());

        // Clear LineEdits
        ui->ledFontName->clear();
        ui->ledColorTexture->clear();
        ui->ledAlphaTexture->clear();
        ui->sbxMaxHeigthOfChars->clear();
        ui->sbxDistanceBetweenChars->clear();
    }

    ui->ledCharacter->clear();
    ui->sbxLeftPixel->clear();
    ui->sbxRightPixel->clear();
    ui->sbxHighestPixelInFontRow->clear();
    ui->ledComment->clear();

    reloadCharList();

    enableFontArea(false);
}

void wFonts::move(int selection, QString action)
{
    if ((selection == 0 && action == "UP") || (selection > font.charList.size() - 1 && action == "DOWN"))
        return;

    int moving;

    if (action == "UP")
        moving = selection - 1;
    else if (action == "DOWN")
        moving = selection + 1;
    else
    {
        qWarning().noquote() << "Invalid move parameter: " + action;
        return;
    }
    setUnsaved();

    // Move the selected item down / up
    font.charList.move(selection, moving);
    reloadCharList();
    ui->lvwChars->setCurrentIndex(strListChars->index(moving));
}

void wFonts::enableFontArea(bool status)
{
    ui->ledCharacter->setEnabled(status);
    ui->sbxLeftPixel->setEnabled(status);
    ui->sbxRightPixel->setEnabled(status);
    ui->sbxHighestPixelInFontRow->setEnabled(status);
    ui->ledComment->setEnabled(status);
    ui->btnDeleteSelection->setEnabled(status);
    ui->btnMoveDown->setEnabled(status);
    ui->btnMoveUp->setEnabled(status);
    ui->actionMoveCharDown->setEnabled(status);
    ui->actionMoveCharUp->setEnabled(status);
    ui->actionDeleteSelection->setEnabled(status);
    ui->actionFindChar->setEnabled(status);
    ui->actionGoToNextError->setEnabled(status);

    if (!status)
    {
        ui->lblCharacter->setStyleSheet("");
        ui->lblRightPixel->setStyleSheet("");
        ui->lblLeftPixel->setStyleSheet("");
        ui->lblHighestPixelInFontRow->setStyleSheet("");
    }
}

void wFonts::open(OTFileMethods::fileMethods method, QString filen, QStringConverter::Encoding encoding)
{
    if ((filen != "") && !QFile(filen).exists())
    {
        if (method != OTFileMethods::silentOpen)
            msg.fileOpenErrorCloseOMSI(this, filen);
        return;
    }

    if (unsaved)
    {
        int msgResult = msg.unsavedChanges(this);
        if (msgResult == -1)
            return;
        else if (msgResult == 1)
            save(OTFileMethods::save, font.path);
    }

    qInfo() << "Open file" << filen;
    qDebug().noquote() << "Open method:" << method;
    if (method == OTFileMethods::open)
    {
        if (filen == "")
        {
            qDebug() << "Open with file dialog";
            filen = QFileDialog::getOpenFileName(this, tr("Open font..."), set.read("main", "mainDir").toString() + "/Fonts", tr("OMSI font file") + " (*.oft)");
            if (filen == "")
                return;
            else
                selectAllAndClear();
        }
        font.path = filen;
    }

    // Make an direct (= more saver) autosave by coping the file
    if (method != OTFileMethods::silentOpen)
    {
        qDebug() << "Create font file backup...";
        QFile::copy(font.path, "backup/font_backup_" + misc.getDate("yyyyMMdd") + "_" + misc.getTime("hhmmss") + " " + font.name + "_afterOpen.oft");
    }

    // Cut out only the file name and put it into the window title
    if (method != OTFileMethods::silentOpen)
    {
        QFileInfo fileInfo(QFile(font.path).fileName());
        QString filenameWithoutPath(fileInfo.fileName());
        setTitle(filenameWithoutPath);
    }

    if (method == OTFileMethods::open)
        saveRecentFiles(QDir().absoluteFilePath(font.path));

    OTFontModel tempFont = font;
    if (method == OTFileMethods::silentOpen)
        tempFont.path = filen;

    selectAllAndClear();
    tempFont = filehandler.openFont(tempFont.path, encoding);
    if (tempFont.error)
    {
        if (method != OTFileMethods::silentOpen)
            msg.fileOpenErrorCloseOMSI(this, tempFont.path);
        return;
    }
    else if (tempFont.moreThanOneFont)
    {
        if (method != OTFileMethods::silentOpen)
            QMessageBox::warning(this, tr("Open font"), tr("Attention: The selected font file contains more than one font. The application cannot read multiple fonts. Please split each font in this file into seperate files.\nNo font will be opened."));

        setUnsaved(false);
        return;
    }

    setUnsaved();

    ui->ledFontName->setText(tempFont.name);
    ui->ledColorTexture->setText(tempFont.colorTexture);
    ui->ledAlphaTexture->setText(tempFont.alphaTexture);

    if (tempFont.maxHeightOfChars != -1)
        ui->sbxMaxHeigthOfChars->setValue(tempFont.maxHeightOfChars);
    else
        ui->sbxMaxHeigthOfChars->clear();

    if (tempFont.distanceBetweenChars != -1)
        ui->sbxDistanceBetweenChars->setValue(tempFont.distanceBetweenChars);
    else
        ui->sbxDistanceBetweenChars->clear();

    font = tempFont;

    reloadCharList();
    ui->lvwChars->setCurrentIndex(strListChars->index(0));

    if (font.charList.size() != 0)
    {
        qDebug().noquote() << QString("Font contains %1 chars.").arg(font.charList.size());
        enableFontArea(true);
        reloadCharUI();
    }
    else
    {
        qDebug() << "Font is empty";
        enableFontArea(false);
    }

    font.error = false;
    setUnsaved(false);

    checkPropValidity();
    checkCharValidity();

    qDebug() << "Font opened.";
}

void wFonts::reloadCharList(bool addChar)
{
    charListUpdate = true;
    qDebug() << "Updating charlist...";
    int currentRow = ui->lvwChars->currentIndex().row();
    qDebug().noquote() << "currentRow:" << currentRow;

    QStringList chars;

    foreach (OTCharacterModel current, font.charList)
        chars << current.character;

    strListChars->setStringList(chars);
    ui->lvwChars->setModel(strListChars);

    if (!addChar)
    {
        if (strListChars->index(currentRow).isValid())
            ui->lvwChars->setCurrentIndex(strListChars->index(currentRow));
        else if (strListChars->index(currentRow - 1).isValid())
            ui->lvwChars->setCurrentIndex(strListChars->index(currentRow - 1));
        else
            ui->lvwChars->setCurrentIndex(strListChars->index(-1));
    }
    else
        ui->lvwChars->setCurrentIndex(strListChars->index(strListChars->rowCount() - 1));


    ui->lblCharCount->setText(QString::number(font.charList.size()));
    charListUpdate = false;
}

QString wFonts::save(OTFileMethods::fileMethods method, QString filen)
{
    qDebug() << "Save font...";

    // Get file name via file dialog if no file was open
    if (method != OTFileMethods::backupSave)
    {
        QString dir;
        if (font.path == "")
        {
            if (font.name == "" || font.name == " ")
                dir = set.read("main", "mainDir").toString() + "/Fonts";
            else
                dir = set.read("main", "mainDir").toString() + "/Fonts/" + font.name + ".oft";
        }
        else
            dir = font.path;

        if (filen == "" || method == OTFileMethods::saveAs)
        {
            qDebug() << "Save font with file dialog";
            filen = QFileDialog::getSaveFileName(this, tr("Save font"), dir, tr("OMSI font file") +  " (*.oft)");
        }
        if (filen == "")
            return "";

        font.path = filen;
    }

    if ((method != OTFileMethods::backupSave) && (font.path == ""))
        return "";

    if (method != OTFileMethods::backupSave)
    {
        QFile file(font.path);

        // Cut out only file name and put it into the window title
        QFileInfo fileInfo(QFile(font.path).fileName());
        QString filenameWithoutPath(fileInfo.fileName());
        setTitle(filenameWithoutPath);
    }

    OTFontModel tempFont = font;
    if (method == OTFileMethods::backupSave)
    {
        if ((font.name != "") || (font.name != " "))
            tempFont.path = QDir().absoluteFilePath("backup/font_backup_" + misc.getDate("yyyyMMdd") + "_" + misc.getTime("hhmmss") + " " + font.name + ".oft");
        else
            tempFont.path = QDir().absoluteFilePath("backup/font_backup_" + misc.getDate("yyyyMMdd") + "_" + misc.getTime("hhmmss") + ".oft");
    }

    qDebug() << "Direct path:" << tempFont.path;

    if (!filehandler.saveFont(tempFont))
    {
        if (method != OTFileMethods::backupSave)
        {
            qWarning() << "Font could not be saved!";
            ui->statusbar->showMessage(tr("Error: The file could not be saved."), 4000);
        }
        return "";
    }


    if (method != OTFileMethods::backupSave)
    {
        saveRecentFiles(QDir().absoluteFilePath(font.path));

        ui->statusbar->showMessage(tr("File saved successfully."), 4000);
        qInfo() << "File successfully saved!";
        qDebug().noquote() << "File: '" + QFileInfo(tempFont.path).absoluteFilePath() + "'";
        setUnsaved(false);
    }
    else
        qDebug().noquote() << "Backup file successfully saved: '" + QFileInfo(tempFont.path).absoluteFilePath() + "'";

    return "";
}

void wFonts::setTitle(QString filen)
{
    if (filen == "empty")
        wFonts::setWindowTitle(OTInformation::name + " - " + tr("fonts"));
    else
        wFonts::setWindowTitle(OTInformation::name + " - " + tr("fonts") + " (" + filen + ")");
}

void wFonts::on_actionNewFont_triggered()
{
    if (unsaved)
    {
        int msgResult = msg.unsavedChanges(this);
        if (msgResult == -1)
            return;
        else if (msgResult == 1)
            save(OTFileMethods::save, font.path);
    }

    qDebug() << "Create new font";

    selectAllAndClear();

    reloadCharList();

    ui->twgFont->setCurrentIndex(0);
    setTitle();
    setUnsaved(false);
}

void wFonts::on_actionFindChar_triggered()
{
    ui->gbxSearchChar->setVisible(true);
    ui->ledSearch->setFocus();
}

void wFonts::on_actionGoToNextError_triggered()
{
    qDebug() << "Go to next error...";
    int currentPos = ui->lvwChars->currentIndex().row();
    if (currentPos == -1)
        currentPos = 0;

    bool secondRound = false;
    int i = 0;
    if (currentPos == font.charList.size() - 1)
        secondRound = true;
    else if (currentPos == 0)
        i = 1;
    else
        i = currentPos + 1;

    QStringList chars;
    forever
    {
        bool rightPixelTooBig = false;
        bool leftPixelTooBig = false;
        bool highestPixelTooBig = false;

        if (QFile(set.read("main", "mainDir").toString() + "/Fonts/" + font.alphaTexture).exists())
        {
            QImage alphaTexture(set.read("main", "mainDir").toString() + "/Fonts/" + font.alphaTexture);

            if (alphaTexture.width() != 0 || alphaTexture.height() != 0)
            {
                if (font.charList.at(i).rightPixel > QString::number(alphaTexture.width()).toInt())
                    rightPixelTooBig = true;

                if (font.charList.at(i).leftPixel > QString::number(alphaTexture.width()).toInt())
                    leftPixelTooBig = true;

                if (font.charList.at(i).highestPixelInFontRow > QString::number(alphaTexture.width()).toInt())
                    highestPixelTooBig = true;
            }
        }

        if (font.charList.at(i).character.isEmpty() || chars.contains(font.charList.at(i).character) ||
           font.charList.at(i).leftPixel == -1 ||
           font.charList.at(i).rightPixel == -1 ||
           font.charList.at(i).highestPixelInFontRow == -1 ||
           font.charList.at(i).leftPixel >= font.charList.at(i).rightPixel || rightPixelTooBig || leftPixelTooBig || highestPixelTooBig)
        {
            ui->lvwChars->setCurrentIndex(strListChars->index(i));
            reloadCharUI();
            ui->twgFont->setCurrentIndex(1);
            qInfo() << QString("Error in char '%1' at position %2").arg(font.charList.at(i).character).arg(i + 1);
            ui->statusbar->showMessage(tr("Error in character '%1'").arg(font.charList.at(i).character), 5000);
            return;
        }

        chars << font.charList.at(i).character;

        // Wenn es voll ist (1. Runde) ODER wenn in der 2. Runde die vorherige Position erreicht wurde:
        // Überprüfung beenden, da Font keine Fehler enthält.
        if ((secondRound && (i == font.charList.size() - 1)) || (secondRound && (i == currentPos)))
            goto end;

        // Wenn es voll ist / 2. Runde
        if (!secondRound && (i == font.charList.size() - 1))
        {
            i = 0;
            secondRound = true;
        }
        else
            i++;
    }

    end:
    ui->statusbar->showMessage(tr("There aren't any errors. The font is valid."), 4000);
    qInfo() << "Could not found any errors.";
}

void wFonts::on_actionDeleteSelection_triggered()
{
    qDebug().noquote() << QString("Delete char '%1' at position %2...").arg(font.charList.at(ui->lvwChars->currentIndex().row()).character).arg(ui->lvwChars->currentIndex().row());

    font.charList.removeAt(ui->lvwChars->currentIndex().row());
    reloadCharList();

    ui->twgFont->setCurrentIndex(1);

    qDebug() << "Font charlist count:" << font.charList.size();

    if (font.charList.size() == 0)
        selectAllAndClear(true);
    else
        reloadCharUI();

    setUnsaved();
}

void wFonts::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPREFERENCES->show();
}

void wFonts::on_actionMoveCharUp_triggered()
{
    if (font.charList.size() != 0)
    {
        move(ui->lvwChars->currentIndex().row(), "UP");
        ui->twgFont->setCurrentIndex(1);
    }
}

void wFonts::on_actionMoveCharDown_triggered()
{
    if (ui->lvwChars->currentIndex().row() != font.charList.size() - 1)
    {
        move(ui->lvwChars->currentIndex().row(), "DOWN");
        ui->twgFont->setCurrentIndex(1);
    }
}

void wFonts::on_actionClose_triggered()
{
    if (unsaved)
    {
        int msgResult = msg.unsavedChanges(this);
        if (msgResult == -1)
            return;
        else if (msgResult == 1)
            save(OTFileMethods::save, font.path);
    }

    close();
}

void wFonts::on_btnColorTexture_clicked()
{
     QString filename = QFileDialog::getOpenFileName(this, tr("Select color texture..."), set.read("main", "mainDir").toString() + "/Fonts", tr("Bitmap picture") + " (*.bmp)");
     QFile file(filename);
     if (filename == "")
         return;

     font.colorTexture = filename.remove(0, QString(set.read("main", "mainDir").toString() + "/Fonts").size() + 1);

     if (ui->ledColorTexture->text() != font.colorTexture)
     {
         qDebug() << QString("New color texture: '%1'").arg(filename);
         setUnsaved();
     }

     ui->ledColorTexture->setText(font.colorTexture);
}

void wFonts::on_btnAlphaTexture_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select alpha texture..."), set.read("main", "mainDir").toString() + "/Fonts", tr("Bitmap picture") + " (*.bmp)");
    if (filename == "")
        return;

    font.alphaTexture = filename.remove(0, QString(set.read("main", "mainDir").toString() + "/Fonts").size() + 1);

    if (ui->ledAlphaTexture->text() != font.alphaTexture)
    {
        qDebug() << QString("New alpha texture: '%1'").arg(filename);
        setUnsaved();
    }

    ui->ledAlphaTexture->setText(font.alphaTexture);
}

void wFonts::on_actionNewChar_triggered()
{
    qDebug() << "Add new char...";
    enableFontArea(true);

    int prevPixelRow;
    if(ui->sbxHighestPixelInFontRow->text() == "")
        prevPixelRow = -1;
    else
        prevPixelRow = ui->sbxHighestPixelInFontRow->value();

    font.charList.append(OTCharacterModel());
    reloadCharList(true);
    setUnsaved();

    reloadCharUI();

    ui->twgFont->setCurrentIndex(1);
    ui->ledCharacter->setFocus();

    if ((set.read(objectName(), "keepPixelRow").toBool()) && (prevPixelRow != -1))
    {
        ui->sbxHighestPixelInFontRow->setValue(prevPixelRow);
        on_sbxHighestPixelInFontRow_textChanged(QString::number(prevPixelRow));
    }
}

void wFonts::on_ledCharacter_textChanged(const QString &arg1)
{
    if (font.charList.size() != 0 && ui->lvwChars->currentIndex().row() != -1)
    {
        font.charList[ui->lvwChars->currentIndex().row()].character = arg1;
        if (!charUIUpdate)
            reloadCharList();
    }

    checkCharValidity();
    setUnsaved();
}

void wFonts::on_ledComment_textChanged(const QString &arg1)
{
    if (font.charList.size() != 0 && ui->lvwChars->currentIndex().row() != -1)
    {
        font.charList[ui->lvwChars->currentIndex().row()].comment = arg1;
    }

    setUnsaved();
}

void wFonts::on_ledFontName_textChanged(const QString &arg1)
{
    font.name = arg1;
    checkPropValidity();
    checkCharValidity();
    setUnsaved();
}

void wFonts::on_ledColorTexture_textChanged(const QString &arg1)
{
    font.colorTexture = arg1;
    checkPropValidity();
    checkCharValidity();
    setUnsaved();

    loadTexPreview();
}

void wFonts::on_ledAlphaTexture_textChanged(const QString &arg1)
{
    font.alphaTexture = arg1;
    checkPropValidity();
    checkCharValidity();
    setUnsaved();

    loadTexPreview();
}

void wFonts::on_sbxMaxHeigthOfChars_textChanged(const QString &arg1)
{
    font.maxHeightOfChars = arg1.toInt();
    checkPropValidity();
    checkCharValidity();
    setUnsaved();
}

void wFonts::on_sbxDistanceBetweenChars_textChanged(const QString &arg1)
{
    font.distanceBetweenChars = arg1.toInt();
    checkPropValidity();
    checkCharValidity();
    setUnsaved();
}

void wFonts::on_lvwChars_pressed(const QModelIndex &index)
{
    Q_UNUSED(index);

    reloadCharList();
    reloadCharUI();
}

void wFonts::charSelectionChanged(const QModelIndex &newSel, const QModelIndex &oldSel)
{
    Q_UNUSED(newSel);
    Q_UNUSED(oldSel);

    if (!charListUpdate)
        on_lvwChars_pressed(QModelIndex());
}

void wFonts::reloadCharUI()
{
    charUIUpdate = true;
    OTCharacterModel character = font.charList.at(ui->lvwChars->currentIndex().row());

    ui->ledCharacter->setText(character.character);

    if (character.leftPixel != -1)
        ui->sbxLeftPixel->setValue(character.leftPixel);
    else
        ui->sbxLeftPixel->clear();

    if (character.rightPixel != -1)
        ui->sbxRightPixel->setValue(character.rightPixel);
    else
        ui->sbxRightPixel->clear();

    if (character.highestPixelInFontRow != -1)
        ui->sbxHighestPixelInFontRow->setValue(character.highestPixelInFontRow);
    else
        ui->sbxHighestPixelInFontRow->clear();

    ui->ledComment->setText(character.comment);

    checkCharValidity();
    charUIUpdate = false;
}

void wFonts::on_actionShowInExplorer_triggered()
{
    if (QFile(font.path).exists())
        misc.openInExplorer(font.path);
    else
        ui->statusbar->showMessage(tr("The font file (still) doesn't exist."), 4000);
}

void wFonts::on_btnFind_clicked()
{
    if (font.charList.size() == 0)
    {
        msg.noCharsInFont(this);
        return;
    }

    QString input = ui->ledSearch->text();
    qDebug().noquote() << "Find char: '" + input + "'";

    currentSearch = input;
    ui->btnNextResult->setEnabled(true);

    // Search for char
    ui->lvwChars->setCurrentIndex(strListChars->index(0));

    for (int i = 0; i < font.charList.size(); i++)
    {
        if (font.charList.at(i).character == input)
        {
            qDebug().noquote() << QString("Char '%1' found at position %2.").arg(input).arg(i + 1);
            reloadCharUI();
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

void wFonts::on_btnNextResult_clicked()
{
    qDebug() << "Go to next search result";
    if (currentSearch != "")
    {
        if (font.charList.size() == 1)
            return;

        int i = ui->lvwChars->currentIndex().row();

        QList<OTCharacterModel> tempList;
        tempList = font.charList;
        tempList.removeAt(ui->lvwChars->currentIndex().row());

        bool secondRound = false;

        if (i >= tempList.size())
        {
            i = 0;
            qDebug() << "Search: End of font, go to top";
            ui->statusbar->showMessage(tr("The end of the font was reached, search from top"), 4000);
            secondRound = true;
        }

        forever
        {
            if (i > tempList.size() - 1)
            {
                qDebug() << "Search: Not other char found";
                ui->statusbar->showMessage(tr("No other character found according to the search criteria."), 4000);
                reloadCharUI();
                return;
            }
            else if (tempList.at(i).character == currentSearch)
            {
                if (i > ui->lvwChars->currentIndex().row() || i == ui->lvwChars->currentIndex().row())
                    i++;
                ui->lvwChars->setCurrentIndex(strListChars->index(i));
                goto end;
            }

            if (!secondRound && (i == tempList.size() - 1))
            {
                i = -1;
                secondRound = true;
            }
            else if (secondRound && (i == ui->lvwChars->currentIndex().row() - 1))
            {
                qDebug() << "Search: Not other char found";
                ui->statusbar->showMessage(tr("No other character found according to the search criteria."), 4000);
                reloadCharUI();
                return;
            }

            i++;
        }
    end:
        if (secondRound)
        {
            qDebug() << "Search: End of font, go to top";
            ui->statusbar->showMessage(tr("The end of the font was reached, search from top"), 4000);
        }

        reloadCharUI();
    }
}

void wFonts::on_btnCloseSearch_clicked()
{
    ui->gbxSearchChar->setVisible(false);
    currentSearch = "";
}

void wFonts::on_ledSearch_textChanged(const QString &arg1)
{
    ui->btnFind->setEnabled(arg1.size() == 1);
    ui->btnNextResult->setEnabled(arg1.size() == 1);
    currentSearch = arg1;
}

void wFonts::on_ledSearch_returnPressed()
{
    on_btnFind_clicked();
}

void wFonts::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::fonts);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}

void wFonts::on_sbxLeftPixel_textChanged(const QString &arg1)
{
    if (font.charList.size() != 0 && ui->lvwChars->currentIndex().row() != -1)
    {
        if (arg1 == "")
            font.charList[ui->lvwChars->currentIndex().row()].leftPixel = -1;
        else
            font.charList[ui->lvwChars->currentIndex().row()].leftPixel = arg1.toInt();

//        if (!charUIUpdate)
//            reloadCharList();
    }

    checkCharValidity();
    setUnsaved();
}

void wFonts::on_sbxRightPixel_textChanged(const QString &arg1)
{
    if (font.charList.size() != 0 && ui->lvwChars->currentIndex().row() != -1)
    {
        if (arg1 == "")
            font.charList[ui->lvwChars->currentIndex().row()].rightPixel = -1;
        else
            font.charList[ui->lvwChars->currentIndex().row()].rightPixel = arg1.toInt();
    }

    checkCharValidity();
    setUnsaved();
}

void wFonts::on_sbxHighestPixelInFontRow_textChanged(const QString &arg1)
{
    if (font.charList.size() != 0 && ui->lvwChars->currentIndex().row() != -1)
    {
        if (arg1 == "")
            font.charList[ui->lvwChars->currentIndex().row()].highestPixelInFontRow = -1;
        else
            font.charList[ui->lvwChars->currentIndex().row()].highestPixelInFontRow = arg1.toInt();
    }

    checkCharValidity();
    setUnsaved();
}

void wFonts::loadTexPreview()
{
    QString tex;

    if (set.read(objectName(), "texPreview").toInt() == 0)
        tex = set.read("main", "mainDir").toString() + "/Fonts/" + font.colorTexture;
    else
        tex = set.read("main", "mainDir").toString() + "/Fonts/" + font.alphaTexture;

    ui->cobxPreviewOptions->setCurrentIndex(set.read(objectName(), "texPreview").toInt());

    if (QFile(tex).exists())
    {
        // 'QGraphicsScene::clear()' is not enough - it doesn't reset the draw aera size
        texPreviewScene = new QGraphicsScene(this);
        ui->grvTexPreview->setScene(texPreviewScene);

        texPreviewScene->addPixmap(tex);
        resizeTexPreview();
    }
}

void wFonts::resizeTexPreview()
{
    ui->grvTexPreview->fitInView(texPreviewScene->sceneRect(), Qt::KeepAspectRatio);
}

void wFonts::on_cobxPreviewOptions_currentIndexChanged(int index)
{
    if (setupFinished)
        set.write(objectName(), "texPreview", index);
    loadTexPreview();
}

void wFonts::on_btnReloadTexPreview_clicked()
{
    loadTexPreview();
}

void wFonts::on_actionDelete_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::information(this, tr("Delete font"), tr("The font will be moved to the trash."), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        QString fontPath = font.path;
        selectAllAndClear();
        QFile::moveToTrash(fontPath);
    }
}

void wFonts::on_actionDuplicateCharacter_triggered()
{
    int currentRow = ui->lvwChars->currentIndex().row();
    on_actionNewChar_triggered();
    qApp->processEvents();

    ui->ledCharacter->setText(font.charList.at(currentRow).character);
    on_ledCharacter_textChanged(font.charList.at(currentRow).character);

    ui->sbxLeftPixel->setValue(font.charList.at(currentRow).leftPixel);
    on_sbxLeftPixel_textChanged(QString::number(font.charList.at(currentRow).leftPixel));

    ui->sbxRightPixel->setValue(font.charList.at(currentRow).rightPixel);
    on_sbxRightPixel_textChanged(QString::number(font.charList.at(currentRow).rightPixel));

    ui->sbxHighestPixelInFontRow->setValue(font.charList.at(currentRow).highestPixelInFontRow);
    on_sbxHighestPixelInFontRow_textChanged(QString::number(font.charList.at(currentRow).highestPixelInFontRow));

    ui->ledComment->setText(font.charList.at(currentRow).comment);
    on_ledComment_textChanged(font.charList.at(currentRow).comment);
}

void wFonts::on_actionOpenWithEncoding_triggered()
{
    wSelectEncoding *selectEncoding = new wSelectEncoding(this);
    selectEncoding->show();
    connect(selectEncoding, &wSelectEncoding::encodingSelected, this, &wFonts::selectedEncoding);
}

void wFonts::selectedEncoding(QStringConverter::Encoding selectedEncoding)
{
    open(OTFileMethods::open, "", selectedEncoding);
}

void wFonts::on_actionBackToHome_triggered()
{
    close();
    backToHome();
}

void wFonts::on_btnEditorPreferences_clicked()
{
    WPREFERENCES = new wPreferences(this, "wFonts");
    WPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPREFERENCES->show();
}
