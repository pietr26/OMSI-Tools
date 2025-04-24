#ifndef VARIABLESSORTFILTERPROXYMODEL_H
#define VARIABLESSORTFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include "VariablesModel.h"

class VariablesSortFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit VariablesSortFilterProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    QString searchString() const;
    void setSearchString(const QString &newFilterString);

    int type() const;
    void setType(int newType);


private:
    QString _searchString;
    int _type;
};

#endif // VARIABLESSORTFILTERPROXYMODEL_H
