#ifndef VARIABLESMODEL_H
#define VARIABLESMODEL_H

#include <QAbstractTableModel>

#include "Project.h"

struct VarModelItem {
    enum Type {
        VarType,
        StringVarType,
        ConstType,
        InvalidType
    };
    VarModelItem() : type(InvalidType), value(0) {}
    VarModelItem(const Type &type, const QString &name, const float &value = 0) : type(type), name(name), value(value) {}

    Type type;
    QString name;
    float value;
};

class VariablesModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit VariablesModel(Project *project, QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    VarModelItem itemAt(const int &row);

protected slots:
    void onVarAdded(const QString &varname);
    void onVarRemoved(const QString &varname);

    void onStringVarAdded(const QString &varname);
    void onStringVarRemoved(const QString &varname);

    void onConstAdded(const QString &constname, const float &value);
    void onConstRemoved(const QString &constname, const float &value);

private:
    Project *_project;

    QList<VarModelItem> _items;
};

#endif // VARIABLESMODEL_H
