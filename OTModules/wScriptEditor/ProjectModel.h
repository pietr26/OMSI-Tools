#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractItemModel>

#include "Project.h"

struct ProjectTreeItem {
    enum Type {
        Root,
        Category,
        File,
        Folder,
        Invalid
    };

    static void addPathItem(const QString &path, ProjectTreeItem *parent);

    Type type;
    QString name, path;
    ProjectTreeItem* parent = nullptr;
    QList<ProjectTreeItem*> children;

    explicit ProjectTreeItem(Type type, const QString &name, const QString &path, ProjectTreeItem* parent = nullptr)
        : type(type), name(name), path(path), parent(parent) {}

    ~ProjectTreeItem() {
        qDeleteAll(children);
    }

    ProjectTreeItem* child(int row) const { return children.value(row); }
    int childCount() const { return children.count(); }
    int row() const { return parent ? parent->children.indexOf(const_cast<ProjectTreeItem*>(this)) : 0; }
};

class ProjectModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit ProjectModel(QObject *parent = nullptr);
    ~ProjectModel();

    void setProject(Project *project);

    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QString filePath(const QModelIndex &index) const;
    ProjectTreeItem::Type type(const QModelIndex &index) const;

protected slots:
    void addCategory(const QString &name, const QStringList &items);

    void reload();

private:
    Project *_project;

    ProjectTreeItem* _invisibleRootItem = nullptr;
    ProjectTreeItem* _rootItem = nullptr;

    QStringList _varlistFiles, _stringVarlistFiles, _scriptFiles, _constFiles;

};

#endif // PROJECTMODEL_H
