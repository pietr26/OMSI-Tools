#include "VariablesModel.h"

VariablesModel::VariablesModel(Project *project, QObject *parent) :
    QAbstractTableModel(parent),
    _project(project) {

    connect(_project, &Project::varAdded,         this, &VariablesModel::onVarAdded);
    connect(_project, &Project::varRemoved,       this, &VariablesModel::onVarRemoved);

    connect(_project, &Project::stringVarAdded,   this, &VariablesModel::onStringVarAdded);
    connect(_project, &Project::stringVarRemoved, this, &VariablesModel::onStringVarRemoved);

    connect(_project, &Project::constAdded,       this, &VariablesModel::onConstAdded);
    connect(_project, &Project::constRemoved,     this, &VariablesModel::onConstRemoved);
}

QVariant VariablesModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation == Qt::Vertical || role != Qt::DisplayRole)
        return QVariant();

    return (section == 0) ? tr("Name") : tr("Value");
}

int VariablesModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;

    return _items.size();
}

int VariablesModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant VariablesModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::DecorationRole && role != Qt::UserRole))
        return QVariant();

    VarModelItem item = _items.at(index.row());

    if(role == Qt::UserRole)
        return item.type;

    if(role ==  Qt::DecorationRole && index.column() == 0) {
        switch(item.type) {
            case VarModelItem::VarType:       return QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew);
            case VarModelItem::StringVarType: return QIcon::fromTheme(QIcon::ThemeIcon::ToolsCheckSpelling);
            case VarModelItem::ConstType:     return QIcon::fromTheme(QIcon::ThemeIcon::EditSelectAll);
            default:                          return QVariant();
        }
    } else if(role == Qt::DecorationRole)
        return QVariant();

    switch(index.column()) {
        case 0: return item.name;
        case 1: return item.value;
    }

    return QVariant();
}

VarModelItem VariablesModel::itemAt(const int &row) {
    if(row < 0 || row >= _items.size())
        return VarModelItem();

    return _items.at(row);
}

void VariablesModel::onVarAdded(const QString &varname) {
    beginInsertRows(QModelIndex(), _items.size(), _items.size());
    _items << VarModelItem(VarModelItem::VarType, varname);
    endInsertRows();
}

void VariablesModel::onVarRemoved(const QString &varname) {
    // FIXME
}

void VariablesModel::onStringVarAdded(const QString &varname) {
    beginInsertRows(QModelIndex(), _items.size(), _items.size());
    _items << VarModelItem(VarModelItem::StringVarType, varname);
    endInsertRows();
}

void VariablesModel::onStringVarRemoved(const QString &varname) {
    // FIXME
}

void VariablesModel::onConstAdded(const QString &constname, const float &value) {
    beginInsertRows(QModelIndex(), _items.size(), _items.size());
    _items << VarModelItem(VarModelItem::ConstType, constname, value);
    endInsertRows();
}

void VariablesModel::onConstRemoved(const QString &constname, const float &value) {
    // FIXME
}
