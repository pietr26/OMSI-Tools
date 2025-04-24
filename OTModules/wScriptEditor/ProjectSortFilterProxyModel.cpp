#include "ProjectSortFilterProxyModel.h"

ProjectSortFilterProxyModel::ProjectSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{}

bool ProjectSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const {
    QString leftText = sourceModel()->data(left, Qt::DisplayRole).toString();
    QString rightText = sourceModel()->data(right, Qt::DisplayRole).toString();

    int leftType = sourceModel()->data(left, Qt::UserRole).toInt();
    int rightType = sourceModel()->data(right, Qt::UserRole).toInt();

    if(leftType == rightType) {
        bool comp = QString::localeAwareCompare(leftText, rightText) < 0;
        if(leftType == ProjectTreeItem::Category)
            return !comp;
        else
            return comp;
    }

    return leftType > rightType;
}
