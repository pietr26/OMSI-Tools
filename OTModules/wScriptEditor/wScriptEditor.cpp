#include "wScriptEditor.h"
#include "ui_wScriptEditor.h"

wScriptEditor::wScriptEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wScriptEditor),
    _project(new Project(this)),
    _projectModel(new ProjectModel(this)) {
    ui->setupUi(this);

    _dwProject = new QDockWidget(tr("Project"), this);
    _docks << _dwProject;

    _projectView = new QTreeView(_dwProject);
    _projectView->setHeaderHidden(true);
    _dwProject->setWidget(_projectView);

    _projectModel->setProject(_project);
    _projectView->setModel(_projectModel);

    addDockWidget(Qt::LeftDockWidgetArea, _dwProject);

    for(QDockWidget *dock : std::as_const(_docks)) {
        dock->setAllowedAreas(dockWidgetArea(dock));
        dock->setFeatures(QDockWidget::DockWidgetClosable);
        dock->setFloating(false);

        ui->menuView->addAction(dock->toggleViewAction());
    }
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

    QFile f(fileName);

    if(!f.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Cannot open file"), tr("Failed opening %1.<br />%2").arg(fileName.split("/").last(), f.errorString()));
        return;
    }

    _project->setPath(fileName);
}
