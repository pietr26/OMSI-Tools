#include "wfonts.h"
#include "ui_wfonts.h"

wFonts::wFonts(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wFonts),
    _font(new OCFont::FontCollection())
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setTitle();

    DiscordGameSDK::setModule(tr("Font creation"));
    DiscordGameSDK::setStatus(tr("Creating a font"));
    DiscordGameSDK::setStart();
    DiscordGameSDK::setIcon("wfonts");
    DiscordGameSDK::update();

    loadRecentFiles();

    ui->statusbar->addPermanentWidget(ui->pgbProgress);
    ui->pgbProgress->setVisible(false);
    centralWidget()->setVisible(false);

    WDGEDITOR = new wdgEditor(this, _font);
    WDGPREVIEW = new wdgPreview(this, _font);

    connect(this, &wFonts::reloadUi, WDGEDITOR, &wdgEditor::reloadUi);
    connect(this, &wFonts::reloadUi, WDGEDITOR, &wdgEditor::switchSelection);
    connect(this, &wFonts::reloadUi, WDGPREVIEW, &wdgPreview::reloadUi);

    connect(this, &wFonts::resizePreview, WDGPREVIEW, &wdgPreview::resizeTexPreview);

    connect(WDGEDITOR, &wdgEditor::reloadPreview, WDGPREVIEW, &wdgPreview::reloadUi);
    connect(WDGEDITOR, &wdgEditor::reloadActionStates, this, &wFonts::setVisiblilty);
    connect(WDGEDITOR, &wdgEditor::setModified, this, &wFonts::setWindowModified);

    createDockWidgets();

    ui->menuEdit->addActions(WDGEDITOR->actionsEdit);

    qInfo().noquote() << objectName() + " started";
}

wFonts::~wFonts()
{
    delete ui;
}

void wFonts::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    emit resizePreview();
}

void wFonts::on_actionBackToHome_triggered()
{
    emit backToHome();
    close();
}

void wFonts::on_actionClose_triggered()
{
    QApplication::quit();
}

void wFonts::createDockWidgets()
{
    QDockWidget *dockEditor = new QDockWidget(tr("Editor"), this);
    dockEditor->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockEditor->setWidget(WDGEDITOR);
    addDockWidget(Qt::TopDockWidgetArea, dockEditor);

    QDockWidget *dockPreview = new QDockWidget(tr("Preview"), this);
    dockPreview->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockPreview->setWidget(WDGPREVIEW);
    addDockWidget(Qt::BottomDockWidgetArea, dockPreview);
}

void wFonts::on_actionNewFile_triggered()
{
    if (!isWindowModified())
    {
        int msgResult = msg.unsavedChanges(this);
        if (msgResult == -1)
            return;
        else if (msgResult == 1)
            save(OTFileMethods::save, _font->path);
    }

    qDebug() << "Create new font";

    //selectAllAndClear();

    emit reloadUi();
    setTitle();

    setWindowModified(false);
}

void wFonts::on_actionOpen_triggered()
{
    open(OTFileMethods::open);
}

void wFonts::on_actionOpenWithEncoding_triggered()
{
    wSelectEncoding *selectEncoding = new wSelectEncoding(this);
    selectEncoding->show();
    connect(selectEncoding, &wSelectEncoding::encodingSelected, this, &wFonts::selectedEncoding);
}

void wFonts::on_actionReload_triggered()
{
    if (!_font->path.isEmpty()) open(OTFileMethods::reopen, _font->path);
}

void wFonts::on_actionSave_triggered()
{
    save(OTFileMethods::save, _font->path);
}

void wFonts::on_actionSaveAs_triggered()
{
    save(OTFileMethods::saveAs, _font->path);
}

void wFonts::on_actionShowInExplorer_triggered()
{
    if (QFile(_font->path).exists()) misc.openInExplorer(_font->path);
    else ui->statusbar->showMessage(tr("The font file (still) doesn't exist."), 4000);
}

void wFonts::save(OTFileMethods::fileMethods method, QString filen)
{
    qDebug() << "Save font...";

    // Get file name via file dialog if no file was open
    if (method != OTFileMethods::backupSave)
    {
        QString dir;
        if (_font->path.isEmpty()) dir = set.read("main", "mainDir").toString() + QString("/Fonts/%1.oft").arg(tr("unnamed"));
        else dir = _font->path;

        if (filen.isEmpty() || method == OTFileMethods::saveAs)
        {
            qDebug() << "Save font with file dialog";
            filen = QFileDialog::getSaveFileName(this, tr("Save font"), dir, tr("OMSI font file") +  " (*.oft)");
        }
        if (filen.isEmpty()) return;

        _font->path = filen;
    }

    if (method != OTFileMethods::backupSave && _font->path.isEmpty())
        return;

    if (method != OTFileMethods::backupSave)
    {
        QFile file(_font->path);

        // Cut out only file name and put it into the window title
        QFileInfo fileInfo(QFile(_font->path).fileName());
        QString filenameWithoutPath(fileInfo.fileName());
        setTitle(filenameWithoutPath);
    }

    OCFont::FontCollection *tempFont = _font;
    if (method == OTFileMethods::backupSave)
    {
        if (_font->path.isEmpty())
            tempFont->path = QDir().absoluteFilePath("backup/font_backup_" + misc.getDate("yyyyMMdd") + "_" + misc.getTime("hhmmss") + QString("_%1.oft").arg("unnamed"));
        else
            tempFont->path = QDir().absoluteFilePath("backup/font_backup_" + misc.getDate("yyyyMMdd") + "_" + misc.getTime("hhmmss") + "_" + QFileInfo(_font->path).fileName());
    }

    qDebug() << "Direct path:" << tempFont->path;

    if (tempFont->write() != OCFont::FontCollection::valid)
    {
        if (method != OTFileMethods::backupSave)
        {
            qWarning() << "Font could not be saved!";
            ui->statusbar->showMessage(tr("Error: The file could not be saved."), 4000);
        }
        return;
    }


    if (method != OTFileMethods::backupSave)
    {
        saveRecentFiles(QDir().absoluteFilePath(_font->path));

        ui->statusbar->showMessage(tr("File saved successfully."), 4000);
        qInfo() << "File successfully saved!";
        qDebug().noquote() << "File: '" + QFileInfo(tempFont->path).absoluteFilePath() + "'";
        setWindowModified(false);
    }
    else qDebug().noquote() << "Backup file successfully saved: '" + QFileInfo(tempFont->path).absoluteFilePath() + "'";

    return;
}

void wFonts::setTitle(QString filen)
{
    if (filen.isEmpty()) wFonts::setWindowTitle("[*] " + tr("Font creation") + " - " + OTInformation::name);
    else wFonts::setWindowTitle("[*] " + filen + " - " + tr("Font creation") + " - " + OTInformation::name);
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

    int i = 0;
    foreach (QString current, recentFiles)
    {
        if (!QFile(current).exists())
            continue;

        QAction *action = new QAction();

        action->setData(current);
        action->setText(QString::number(i + 1) + "      " + QFile(current).fileName());
        action->setVisible(true);
        switch (i)
        {
        case 0: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_1)); break;
        case 1: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_2)); break;
        case 2: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_3)); break;
        case 3: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_4)); break;
        case 4: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_5)); break;
        case 5: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_6)); break;
        case 6: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_7)); break;
        case 7: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_8)); break;
        case 8: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_9)); break;
        case 9: action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0)); break;
        }

        connect(action, &QAction::triggered, this, [=]() { this->open(OTFileMethods::open, action->data().toString()); });

        ui->menuRecentlyOpenedFonts->addAction(action);
        i++;
    }

    if (i == 0) ui->menuRecentlyOpenedFonts->setEnabled(false);
}

void wFonts::saveRecentFiles(QString absoluteNewFilePath)
{
    qDebug() << "Save recent files...";
    QStringList recentFiles = set.read(objectName(), "recentFiles").toStringList();

    int i = 0;
    foreach (QString current, recentFiles)
    {
        if (!QFile(current).exists()) recentFiles.removeAt(i);
        else i++;
    }

    if (recentFiles.contains(absoluteNewFilePath))
        recentFiles.removeAt(recentFiles.indexOf(absoluteNewFilePath));

    recentFiles.prepend(absoluteNewFilePath);

    while (recentFiles.size() > maxRecentFileCount)
        recentFiles.removeLast();

    set.write(objectName(), "recentFiles", recentFiles);

    loadRecentFiles();
}

void wFonts::open(OTFileMethods::fileMethods method, QString filen, QStringConverter::Encoding encoding)
{
    if (!filen.isEmpty() && !QFile(filen).exists())
    {
        if (method != OTFileMethods::silentOpen) msg.fileOpenErrorCloseOMSI(this, filen);
        return;
    }

    if (isWindowModified() == true)
    {
        int msgResult = msg.unsavedChanges(this);
        if (msgResult == -1) return;
        else if (msgResult == 1) save(OTFileMethods::save, _font->path);
    }

    _font->clear();
    _font->path = filen;

    if (method == OTFileMethods::open)
    {
        if (_font->path.isEmpty())
        {
            qDebug() << "Open with file dialog";
            _font->path = QFileDialog::getOpenFileName(this, tr("Open font..."), set.read("main", "mainDir").toString() + "/Fonts", tr("OMSI font file") + " (*.oft)");
            if (_font->path.isEmpty()) return;
            else emit reloadUi();
        }

        saveRecentFiles(QDir().absoluteFilePath(_font->path));
    }

    qInfo() << "Open file" << _font->path;

    if (method != OTFileMethods::silentOpen)
    {
        // Cut out only the file name and put it into the window title
        QFileInfo fileInfo(QFile(_font->path).fileName());
        QString filenameWithoutPath(fileInfo.fileName());
        setTitle(filenameWithoutPath);
    }

    _font->encoding = encoding;
    if (_font->read() != OCFont::FontCollection::valid)
    {
        if (method != OTFileMethods::silentOpen) msg.fileOpenErrorCloseOMSI(this, _font->path);
        return;
    }

    WDGEDITOR->unexpandAll();
    emit reloadUi(true);
    setWindowModified(false);

    qDebug() << "Font opened.";
}

void wFonts::selectedEncoding(QStringConverter::Encoding selectedEncoding)
{
    open(OTFileMethods::open, "", selectedEncoding);
}

void wFonts::setVisiblilty()
{
    ui->actionReload->setEnabled(_font->path.isEmpty() || !QFile(_font->path).exists());
    ui->actionShowInExplorer->setEnabled(_font->path.isEmpty() || !QFile(_font->path).exists());
}

void wFonts::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::fonts);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}
