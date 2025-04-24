#ifndef PROJECTSORTFILTERPROXYMODEL_H
#define PROJECTSORTFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

#include "ProjectModel.h"

class ProjectSortFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit ProjectSortFilterProxyModel(QObject *parent = nullptr);

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // PROJECTSORTFILTERPROXYMODEL_H
