#include "wScriptEditor.h"
#include "ui_wScriptEditor.h"

wScriptEditor::wScriptEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wScriptEditor),
    _toolBar(new QToolBar(this)),
    _project(new Project(this)),
    _projectModel(new ProjectModel(this)),
    _projectProxyModel(new ProjectSortFilterProxyModel(this)) {
    ui->setupUi(this);

    addToolBar(_toolBar);

    ui->tabBar->setExpanding(false);
    ui->tabBar->setTabsClosable(true);

    _toolBar->addAction(ui->actionOpenProject);
    _toolBar->addAction(ui->actionNewFile);
    _toolBar->addAction(ui->actionSave);

    ui->stackedWidget->setCurrentIndex(0);

    _dwProject = new QDockWidget(tr("Project"), this);
    _docks << _dwProject;

    _projectView = new QTreeView(_dwProject);
    _projectView->setHeaderHidden(true);
    _dwProject->setWidget(_projectView);

    _projectModel->setProject(_project);
    _projectProxyModel->setSourceModel(_projectModel);
    _projectProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    _projectView->setModel(_projectProxyModel);
    _projectView->sortByColumn(0, Qt::AscendingOrder);

    addDockWidget(Qt::LeftDockWidgetArea, _dwProject);


    _dwVariables = new QDockWidget(tr("Variables"), this);
    _wdgVariables = new WdgVariables(_project, _dwVariables);
    _dwVariables->setWidget(_wdgVariables);
    _docks << _dwVariables;
    addDockWidget(Qt::RightDockWidgetArea, _dwVariables);


    for(QDockWidget *dock : std::as_const(_docks)) {
        dock->setAllowedAreas(dockWidgetArea(dock));
        dock->setFeatures(QDockWidget::DockWidgetClosable);
        dock->setFloating(false);

        ui->menuView->addAction(dock->toggleViewAction());
    }

    connect(_projectView, &QAbstractItemView::doubleClicked, this, &wScriptEditor::onFileOpen);
    connect(ui->tabBar, &QTabBar::tabCloseRequested, this, &wScriptEditor::onFileClose);
    connect(ui->tabBar, &QTabBar::currentChanged, this, &wScriptEditor::onCurrentFileChanged);
}

wScriptEditor::~wScriptEditor() {
    delete ui;
}

void wScriptEditor::on_actionBackToHome_triggered() {
    if(close())
        emit backToHome();
}

void wScriptEditor::on_actionClose_triggered() {
    close();
}

void wScriptEditor::on_actionOpenProject_triggered() {
    // TODO: close current project

    OTSettings set;
    const QString fileName = QFileDialog::getOpenFileName(this, "", set.read("main", "mainDir").toString(), tr("Project Files (*.bus *.ovh *.sco *.hum)"));
    if(fileName.isEmpty())
        return;

    while(ui->tabBar->count() > 0)
        onFileClose(ui->tabBar->count() - 1);

    QFile f(fileName);

    if(!f.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Cannot open file"), tr("Failed opening %1.<br />%2").arg(fileName.split("/").last(), f.errorString()));
        return;
    }

    _project->setPath(fileName);
    _projectView->expandToDepth(0);
}

void wScriptEditor::onFileOpen(const QModelIndex &index) {
    const QModelIndex sourceIndex = _projectProxyModel->mapToSource(index);
    if(_projectModel->type(sourceIndex) != ProjectTreeItem::File)
        return;

    QString relativeFilePath = _projectModel->filePath(sourceIndex);
    const QString filePath = _project->dir() + "/" + relativeFilePath;

    const int fileIndex = _openFiles.indexOf(filePath);
    if(fileIndex != -1) {
        ui->tabBar->setCurrentIndex(fileIndex);
        return;
    }

    const QString shortName = relativeFilePath.remove("script/");

    QFile f(filePath);
    if(!f.open(QFile::ReadOnly)) {
        qWarning() << "Failed reading" << filePath; // TODO
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream s(&f);
    s.setEncoding(QStringConverter::Latin1);
    const QString text = s.readAll();
    f.close();

    QTextDocument *doc = new QTextDocument(this);
    doc->setDocumentLayout(new QPlainTextDocumentLayout(doc));
    doc->setPlainText(text);
    doc->setDefaultFont(codeFont);

    int tabWidthChars = 3;
    QFontMetricsF metrics(codeFont);
    QTextOption option;
    option.setTabStopDistance(metrics.horizontalAdvance(" ") * tabWidthChars);
    doc->setDefaultTextOption(option);
    QSyntaxHighlighter *highlighter = new ScriptSyntaxHighlighter(codeFont, doc);

    _openFiles << filePath;
    _openDocuments << doc;
    ui->tabBar->addTab(shortName);
    ui->tabBar->setCurrentIndex(ui->tabBar->count() - 1);
    updateStackedWidget();

    qApp->processEvents(); // stay in the override cursor
    QApplication::restoreOverrideCursor();
}

void wScriptEditor::onCurrentFileChanged(const int &tabIndex) {
    if(tabIndex >= 0)
        ui->plainTextEdit->setDocument(_openDocuments[tabIndex]);
    else
        ui->plainTextEdit->setDocument(nullptr);
}

void wScriptEditor::onFileClose(const int &tabIndex) {
    // TODO: check
    _openFiles.remove(tabIndex);
    _openDocuments.takeAt(tabIndex)->deleteLater();
    ui->tabBar->removeTab(tabIndex);
    updateStackedWidget();
}

void wScriptEditor::updateStackedWidget() {
    ui->stackedWidget->setCurrentIndex(static_cast<bool>(ui->tabBar->count()));
}

void wScriptEditor::on_actionGoToLineNumber_triggered() {
    bool ok;
    const int lineNumber = QInputDialog::getInt(this, tr("Go to line number"), tr("Line number:"), 1, 1, ui->plainTextEdit->blockCount(), 1, &ok);
    if(!ok)
        return;

    QTextCursor cursor = ui->plainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumber - 1);
    ui->plainTextEdit->setTextCursor(cursor);
}
