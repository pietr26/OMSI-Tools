#ifndef WSCRIPTEDITOR_H
#define WSCRIPTEDITOR_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTreeView>
#include <QFileDialog>
#include <QToolBar>
#include <QTextDocument>

#include "Project.h"
#include "ProjectModel.h"
#include "ProjectSortFilterProxyModel.h"
#include "WdgVariables.h"

#include "OTBackend/OTGlobal.h"

namespace Ui {
class wScriptEditor;
}

class wScriptEditor : public QMainWindow {
    Q_OBJECT

public:
    explicit wScriptEditor(QWidget *parent = nullptr);
    ~wScriptEditor();

signals:
    void backToHome();

protected slots:
    void on_actionBackToHome_triggered();
    void on_actionClose_triggered();
    void on_actionOpenProject_triggered();

    void onFileOpen(const QModelIndex &index);
    void onCurrentFileChanged(const int &tabIndex);
    void onFileClose(const int &tabIndex);

    void updateStackedWidget();

private:
    Ui::wScriptEditor *ui;

    QToolBar *_toolBar;

    Project *_project;
    ProjectModel *_projectModel;
    ProjectSortFilterProxyModel *_projectProxyModel;

    QList<QDockWidget *> _docks;

    QDockWidget *_dwProject;
    QTreeView *_projectView;

    QDockWidget *_dwVariables;
    WdgVariables *_wdgVariables;

    QStringList _openFiles;
    QList<QTextDocument *> _openDocuments;
};

#endif // WSCRIPTEDITOR_H
