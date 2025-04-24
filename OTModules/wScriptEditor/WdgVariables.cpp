#include "WdgVariables.h"
#include "ui_WdgVariables.h"

WdgVariables::WdgVariables(Project *project, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WdgVariables),
    _model(new VariablesModel(project, this)),
    _proxyModel(new VariablesSortFilterProxyModel(this)) {
    ui->setupUi(this);

    ui->tabBar->addTab(tr("Numeric Variables"));
    ui->tabBar->addTab(tr("String Variables"));
    ui->tabBar->addTab(tr("Constans"));

    _proxyModel->setSourceModel(_model);
    _proxyModel->setSortRole(Qt::DisplayRole);

    ui->twVars->setModel(_proxyModel);

    ui->twVars->setSortingEnabled(true);
    ui->twVars->sortByColumn(0, Qt::AscendingOrder);

    connect(ui->leSearch, &QLineEdit::textChanged, _proxyModel, &VariablesSortFilterProxyModel::setSearchString);
    connect(ui->tabBar, &QTabBar::currentChanged, _proxyModel, &VariablesSortFilterProxyModel::setType);
    connect(ui->tabBar, &QTabBar::currentChanged, this, &WdgVariables::updateColumnCount);

    ui->twVars->setColumnWidth(0, 225);
    ui->twVars->setColumnWidth(1, 50);
    _proxyModel->setType(VarModelItem::VarType);
    updateColumnCount();
}

WdgVariables::~WdgVariables() {
    delete ui;
}

void WdgVariables::updateColumnCount() {
    if(ui->tabBar->currentIndex() == 2) {
        ui->twVars->setColumnHidden(1, false);
    } else {
        ui->twVars->setColumnHidden(1, true);
    }
}
