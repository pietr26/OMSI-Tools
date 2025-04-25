#include "ProjectModel.h"

void ProjectTreeItem::ProjectTreeItem::addPathItem(const QString &path, ProjectTreeItem *parent) {
    QString cleanPath = path;
    cleanPath.remove("script/");

    const QStringList parts = cleanPath.split('/', Qt::SkipEmptyParts);
    ProjectTreeItem *currentParent = parent;

    for (int i = 0; i < parts.size(); ++i) {
        const QString &part = parts[i];

        // Prüfe, ob dieser Name schon unter currentParent existiert
        ProjectTreeItem *existing = nullptr;
        for (ProjectTreeItem *child : std::as_const(currentParent->children)) {
            if (child->name == part) {
                existing = child;
                break;
            }
        }

        if (existing) {
            currentParent = existing;
        } else {
            // Neuen Eintrag erstellen
            ProjectTreeItem *newItem = new ProjectTreeItem(File, part, path, currentParent);
            currentParent->children.append(newItem);
            if(currentParent->type != Category)
                currentParent->type = Folder;
            currentParent = newItem;
        }
    }
}


ProjectModel::ProjectModel(QObject *parent)
    : QAbstractItemModel(parent)
{}

ProjectModel::~ProjectModel() {
    delete _invisibleRootItem;
}

void ProjectModel::setProject(Project *project) {
    qInfo() << "set" << project->path();
    _project = project;
    connect(_project, &Project::reset, this, &ProjectModel::reload);
    reload();
};

QModelIndex ProjectModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ProjectTreeItem* parentItem = parent.isValid()
                                     ? static_cast<ProjectTreeItem*>(parent.internalPointer())
                                     : _invisibleRootItem;  // Sichtbares Root liegt unter Dummy

    ProjectTreeItem* childItem = parentItem->child(row);
    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}


QModelIndex ProjectModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) return QModelIndex();

    ProjectTreeItem* childItem = static_cast<ProjectTreeItem*>(index.internalPointer());
    ProjectTreeItem* parentItem = childItem->parent;

    if (parentItem == _invisibleRootItem || !parentItem) return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectModel::rowCount(const QModelIndex &parent) const {
    if(_project->path().isEmpty())
        return 0;

    ProjectTreeItem* parentItem = parent.isValid() ?
                                      static_cast<ProjectTreeItem*>(parent.internalPointer()) :
                                      _invisibleRootItem;
    return parentItem->childCount();
}

int ProjectModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::DecorationRole && role != Qt::UserRole))
        return QVariant();

    ProjectTreeItem* item = static_cast<ProjectTreeItem*>(index.internalPointer());

    if(role == Qt::UserRole)
        return item->type;

    if(role == Qt::DecorationRole) {
        switch(item->type) {
            case ProjectTreeItem::Root: return QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties);
            case ProjectTreeItem::Category: return QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen);
            case ProjectTreeItem::Folder: return QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen);
            case ProjectTreeItem::File: return QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew);
        }
    }

    QString name = item->name;
    if(name.startsWith("script/"))
        name.remove("script/", Qt::CaseInsensitive);
    return name;
}

QString ProjectModel::filePath(const QModelIndex &index) const {
    if(!index.isValid())
        return "";

    return static_cast<ProjectTreeItem *>(index.internalPointer())->path;
}

ProjectTreeItem::Type ProjectModel::type(const QModelIndex &index) const {
    if(!index.isValid())
        return ProjectTreeItem::Invalid;

    return static_cast<ProjectTreeItem *>(index.internalPointer())->type;
}

void ProjectModel::addCategory(const QString &name, const QStringList &items) {
    ProjectTreeItem *category = new ProjectTreeItem(ProjectTreeItem::Category, name, "", _rootItem);
    _rootItem->children.append(category);

    for (const QString& path : items)
        ProjectTreeItem::addPathItem(path, category);
}

void ProjectModel::reload() {
    beginResetModel();
    if(_invisibleRootItem)
        delete _invisibleRootItem;
    _invisibleRootItem = new ProjectTreeItem(ProjectTreeItem::Root, "", "");
    _rootItem = new ProjectTreeItem(ProjectTreeItem::Root, _project->path().split("/").last(), "");
    _invisibleRootItem->children.append(_rootItem);

    _varlistFiles = _project->varlistFiles().values();
    _stringVarlistFiles = _project->stringVarlistFiles().values();
    _scriptFiles = _project->scriptFiles().values();
    _constFiles = _project->constFiles().values();

    addCategory(tr("Varlists"), _varlistFiles);
    addCategory(tr("Stringvarlists"), _stringVarlistFiles);
    addCategory(tr("Script files"), _scriptFiles);
    addCategory(tr("Constfiles"), _constFiles);

    endResetModel();
}
