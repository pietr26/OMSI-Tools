#include "wregeditor.h"
#include "ui_wregeditor.h"

wRegEditor::wRegEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wRegEditor)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    resize(misc.sizeWindow(0.5, 0.6));
    qDebug() << "UI set";

    setWindowTitle("RegEditor - " + OTInformation::name);

    loadRegs();

    qInfo().noquote() << objectName() + " started";
}

wRegEditor::~wRegEditor()
{
    delete ui;
}

void wRegEditor::on_twgRegEditor_cellChanged(int row, int column)
{
    Q_UNUSED(column);

    if (loadRegsFinished)
    {
        QStringList setPath = ui->twgRegEditor->item(row, 0)->text().split("/");
        bool isNumeric; ui->twgRegEditor->item(row, 2)->text().toInt(&isNumeric);

        set.write(setPath.join("/").replace("/" + setPath.last(), ""), setPath.last(), (isNumeric) ? QVariant(ui->twgRegEditor->item(row, 2)->text().toInt()) : ui->twgRegEditor->item(row, 2)->text());
        ui->statusbar->showMessage("Saved: " + ui->twgRegEditor->item(row, 0)->text() + " = " + ui->twgRegEditor->item(row, 2)->text());
    }
}

void wRegEditor::on_btnReload_clicked()
{
    loadRegs();
}

void wRegEditor::loadRegs()
{
    loadRegsFinished = false;

    QList<QPair<QString, QVariant>> regs = set.getAllPreferences();
    ui->twgRegEditor->setRowCount(regs.count());

    for (int i = 0; i < regs.count(); i++)
    {
        ui->twgRegEditor->setItem(i, 0, new QTableWidgetItem(regs.at(i).first));
        ui->twgRegEditor->item(i, 0)->setFlags(ui->twgRegEditor->item(i, 0)->flags() & ~Qt::ItemIsEditable); // set non-editable for single column

        ui->twgRegEditor->setItem(i, 1, new QTableWidgetItem(regs.at(i).second.typeName()));
        ui->twgRegEditor->item(i, 1)->setFlags(ui->twgRegEditor->item(i, 0)->flags() & ~Qt::ItemIsEditable); // set non-editable for single column

        if (std::strcmp(regs.at(i).second.typeName(), "QStringList") == 0)
        {
            ui->twgRegEditor->setItem(i, 2, new QTableWidgetItem("[not editable]"));
            ui->twgRegEditor->item(i, 2)->setFlags(ui->twgRegEditor->item(i, 0)->flags() & ~Qt::ItemIsEditable); // set non-editable for single column
        }
        else
            ui->twgRegEditor->setItem(i, 2, new QTableWidgetItem(regs.at(i).second.toString()));
    }

    ui->twgRegEditor->resizeColumnsToContents();
    ui->twgRegEditor->resizeRowsToContents();

    loadRegsFinished = true;
}

void wRegEditor::on_actionBackToHome_triggered()
{
    emit backToHome();
    close();
}

void wRegEditor::on_actionClose_triggered()
{
    close();
}
