#ifndef WDGVARIABLES_H
#define WDGVARIABLES_H

#include <QWidget>
#include <QSortFilterProxyModel>

#include "VariablesModel.h"
#include "VariablesSortFilterProxyModel.h"

namespace Ui {
class WdgVariables;
}

class WdgVariables : public QWidget {
    Q_OBJECT

public:
    explicit WdgVariables(Project *project, QWidget *parent = nullptr);
    ~WdgVariables();

protected:
    void updateColumnCount();

private:
    Ui::WdgVariables *ui;
    VariablesModel *_model;
    VariablesSortFilterProxyModel *_proxyModel;
};

#endif // WDGVARIABLES_H
