#include "dlgfilepreview.h"
#include "ui_dlgfilepreview.h"

#include "OTBackend/OTGlobal.h"
#include <QMessageBox>
#include <QFile>
#include <QUrl>
#include <QDesktopServices>
#include <QLabel>

dlgFilePreview::dlgFilePreview(QWidget *parent, const QString &filePath, const OTContentValidatorResult &result) :
    QDialog(parent),
    ui(new Ui::dlgFilePreview) {
    ui->setupUi(this);

    ui->twgFiles->setColumnHidden(1, true);

    ui->twgPreview->setColumnHidden(0, true);
    ui->twgPreview->setColumnWidth(1, 50);

    ui->splitter->setSizes({150});

    QAction *closeAction = ui->twgFiles->addAction(QIcon::fromTheme(QIcon::ThemeIcon::WindowClose),
                                                   tr("Close"),
                                                   QKeySequence(Qt::CTRL | Qt::Key_W));
    closeAction->setShortcutContext(Qt::WindowShortcut);

    QAction *openExternallyAction = ui->twgFiles->addAction(QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen),
                                                            tr("Open externally"),
                                                            QKeySequence(Qt::CTRL | Qt::Key_Return));
    openExternallyAction->setShortcutContext(Qt::WindowShortcut);

    connect(openExternallyAction, &QAction::triggered, this, &dlgFilePreview::on_btnOpenFile_clicked);
    connect(closeAction, &QAction::triggered, this, [this](){
        delete ui->twgFiles->currentItem();
    });

    ui->twgFiles->setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction *close = addAction("", QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_W));
    connect(close, &QAction::triggered, this, &dlgFilePreview::close);

    openFile(filePath, result);
}

dlgFilePreview::~dlgFilePreview() {
    delete ui;
}

void dlgFilePreview::openFile(const QString &filePath,const OTContentValidatorResult &result) {
    if(isBinaryFile(filePath)) {
        openExternally(filePath);
        return;
    }

    QTreeWidgetItem *currentItem = ui->twgFiles->currentItem();
    if(!currentItem && ui->twgFiles->topLevelItemCount() != 0)
        return;

    // check, if the file is already opened
    if(currentItem) {
        for(int i = 0; i < currentItem->childCount(); i++) {
            QTreeWidgetItem *itm = currentItem->child(i);
            if(itm->text(1) == filePath) {
                ui->twgFiles->setCurrentItem(itm);
                return;
            }
        }
    }

    QTreeWidgetItem *newItem = new QTreeWidgetItem;

    QFileInfo fi(filePath);
    fi.fileName();
    newItem->setText(0, fi.fileName());
    newItem->setText(1, filePath);
    _results[filePath] = result;
    if(ui->twgFiles->topLevelItemCount() != 0)
        currentItem->addChild(newItem);
    else
        ui->twgFiles->addTopLevelItem(newItem);
    ui->twgFiles->setCurrentItem(newItem);
}

void dlgFilePreview::loadFile(const QString &filePath) {
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    clear();
    QString localFilePath = filePath;
    OTSettings set;
    setWindowTitle(localFilePath.remove(set.read("main", "mainDir").toString() + "/").replace("/", "\\"));

    QFile f(filePath);
    if(!f.exists()) {
        qWarning() << "File not found: " << filePath;
        QMessageBox::critical(this, tr("File not found"), tr("The file could not be found!"));
        return;
    }

    if(!f.open(QFile::ReadOnly)) {
        qWarning().noquote() << "Could not open file:" << filePath;
        QMessageBox::critical(this, tr("Couldn't open file"), tr("The file could not be opened!"));
        return;
    }

    QTextStream s(&f);
    if(filePath.endsWith(".map"))
        s.setEncoding(QStringConverter::Utf16LE);
    else
        s.setEncoding(QStringConverter::Latin1);

    QString text = s.readAll();
    f.close();

    QStringList lines = text.split("\r\n");
    int i = 0;
    for(QString line : lines) {
        i++;
        QLabel *label = new QLabel(line);
        QLabel *numberLabel = new QLabel("<html style=\"color: #888888\"><table width=\"100%\"><tr><td align=\"right\">" + QString::number(i) + " </td></tr></table></html>");
        label->setFixedHeight(16);
        numberLabel->setFixedHeight(16);

        QTreeWidgetItem *itm = new QTreeWidgetItem;
        ui->twgPreview->addTopLevelItem(itm);
        ui->twgPreview->setItemWidget(itm, 1, numberLabel);
        ui->twgPreview->setItemWidget(itm, 2, label);
    }

    _currentResult = _results[filePath];

    QList<OTContentValidatorIssue> issues = _currentResult.issues();

    QHash<int, QString> linkedFiles = _currentResult.linkedFiles();
    for (QHash<int, QString>::const_iterator it = linkedFiles.constBegin(); it != linkedFiles.constEnd(); ++it) {
        int line = it.key();
        QString file = it.value();

        QTreeWidgetItem *itm = ui->twgPreview->topLevelItem(line - 1);
        if(!itm)
            continue;

        QLabel *label = qobject_cast<QLabel*>(ui->twgPreview->itemWidget(itm, 2));
        QString colorCode = OTSettings::currentColorScheme() == Qt::ColorScheme::Dark ? "#00aaff" : "#0055ff";
        label->setText("<u style=\"color: " + colorCode + ";\">" + label->text() + "</u>");
    }

    for(OTContentValidatorIssue issue : issues) {
        int line = issue.lineNumber();
        QTreeWidgetItem *itm = ui->twgPreview->topLevelItem(line - 1);
        if(!itm)
            continue;

        QString colorCode;
        if(issue.isWarning()) {
            itm->setBackground(1, QColor(255, 170, 0, 64));
            itm->setBackground(2, QColor(255, 170, 0, 64));
            colorCode = OTSettings::currentColorScheme() == Qt::ColorScheme::Dark ? "#ffaa00" : "#666600";
        } else {
            itm->setBackground(1, QColor(255, 0, 0, 64));
            itm->setBackground(2, QColor(255, 0, 0, 64));
            colorCode = OTSettings::currentColorScheme() == Qt::ColorScheme::Dark ? "#FF6666" : "#AA0000";
        }
        itm->setToolTip(2, issue.longissueDescription());

        QLabel *label = qobject_cast<QLabel*>(ui->twgPreview->itemWidget(itm, 2));


        label->setText("<html><table width=\"100%\"><tr><td>" + label->text() + "</td><td align=\"right\" style=\"color: " + colorCode + ";\">" + issue.shortissueDescription() + "</td></tr></table></html>");
    }

    qApp->restoreOverrideCursor();
}

void dlgFilePreview::clear() {
    ui->twgPreview->clear();
    setWindowTitle(tr("No file opened"));
}

void dlgFilePreview::on_btnOpenFile_clicked() {
    QTreeWidgetItem *itm = ui->twgFiles->currentItem();
    if(!itm)
        return;

    openExternally(itm->text(1));
}

void dlgFilePreview::openExternally(QString filePath) {
    QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(filePath.replace("\\", "/"))));
}

bool dlgFilePreview::isBinaryFile(const QString &filePath) {
    return _binaryFileTypes.contains(filePath.split(".").last());
}

void dlgFilePreview::on_twgFiles_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    Q_UNUSED(previous);
    if(!current)
        clear();
    else
        loadFile(current->text(1));
}

void dlgFilePreview::on_twgPreview_itemClicked(QTreeWidgetItem *item, int column) {
    Q_UNUSED(column);
    if(!item)
        return;

    int line = ui->twgPreview->indexOfTopLevelItem(item) + 1;
    QHash<int, QString> linkedFiles = _currentResult.linkedFiles();
    if(linkedFiles.contains(line)) {
        // TODO: Implement proper sub-content validator result which is not implemented now!
        openFile(linkedFiles[line], OTContentValidatorResult());
    }
}

void dlgFilePreview::on_btnOpenDirectory_clicked() {
    QTreeWidgetItem *itm = ui->twgFiles->currentItem();
    if(!itm)
        return;

    QString filePath = itm->text(1);
    QFileInfo fi(filePath);
    QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(fi.dir().path().replace("\\", "/"))));
}
