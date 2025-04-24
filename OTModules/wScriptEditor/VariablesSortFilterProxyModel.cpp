#include "VariablesSortFilterProxyModel.h"

VariablesSortFilterProxyModel::VariablesSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{}

bool VariablesSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    VarModelItem item = static_cast<VariablesModel *>(sourceModel())->itemAt(sourceRow);
    return item.type == _type && item.name.contains(_searchString, Qt::CaseInsensitive);
}

QString VariablesSortFilterProxyModel::searchString() const {
    return _searchString;
}

void VariablesSortFilterProxyModel::setSearchString(const QString &newFilterString) {
    _searchString = newFilterString;
    invalidateFilter();
}

int VariablesSortFilterProxyModel::type() const {
    return _type;
}

void VariablesSortFilterProxyModel::setType(int newType) {
    _type = newType;
    invalidateFilter();
}

