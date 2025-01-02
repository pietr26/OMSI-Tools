#include "dlgfilepreview.h"
#include "ui_dlgfilepreview.h"

#include "OTBackend/OTGlobal.h"
#include <QMessageBox>
#include <QFile>
#include <QUrl>
#include <QDesktopServices>
#include <QLabel>

dlgFilePreview::dlgFilePreview(QWidget *parent, const QString &filePath, const OTContentValidatorResult &result) :
    QDialog(parent),
    ui(new Ui::dlgFilePreview),
    _filePath(filePath),
    _result(result) {
    ui->setupUi(this);

    ui->twgPreview->setColumnHidden(0, true);
    ui->twgPreview->setColumnWidth(1, 50);

    QString localFilePath = filePath;

    OTSettings set;
    setWindowTitle(localFilePath.remove(set.read("main", "mainDir").toString() + "/"));
    loadFile();
}

dlgFilePreview::~dlgFilePreview() {
    delete ui;
}

void dlgFilePreview::loadFile() {
    QFile f(_filePath);
    if(!f.exists()) {
        qWarning() << "File not found: " << _filePath;
        QMessageBox::critical(this, tr("File not found"), tr("The file could not be found!"));
        return;
    }

    if(!f.open(QFile::ReadOnly)) {
        qWarning() << "Could not open file: " << _filePath;
        QMessageBox::critical(this, tr("Couldn't open file"), tr("The file could not be opened!"));
        return;
    }

    QTextStream s(&f);
    QString text = s.readAll();
    f.close();

    QStringList lines = text.split("\r\n");
    int i = 0;
    for(QString line : lines) {
        i++;
        QLabel *label = new QLabel(line);
        QLabel *numberLabel = new QLabel("<html style=\"color: #888888\"><table width=\"100%\"><tr><td align=\"right\">" + QString::number(i) + " </td></tr></table></html>");
        label->setFixedHeight(16);
        numberLabel->setFixedHeight(16);

        QTreeWidgetItem *itm = new QTreeWidgetItem;
        ui->twgPreview->addTopLevelItem(itm);
        ui->twgPreview->setItemWidget(itm, 1, numberLabel);
        ui->twgPreview->setItemWidget(itm, 2, label);
    }

    QList<OTContentValidatorIssue> issues = _result.issues();

    for(OTContentValidatorIssue issue : issues) {
        int line = issue.lineNumber();
        QTreeWidgetItem *itm = ui->twgPreview->topLevelItem(line - 1);
        if(!itm)
            return;

        itm->setBackground(1, QColor(255, 0, 0, 64));
        itm->setBackground(2, QColor(255, 0, 0, 64));
        itm->setToolTip(2, issue.longissueDescription());

        QLabel *label = qobject_cast<QLabel*>(ui->twgPreview->itemWidget(itm, 2));

        label->setText("<html><table width=\"100%\"><tr><td>" + label->text() + "</td><td align=\"right\" style=\"color: " + (OTSettings::currentColorScheme() == Qt::ColorScheme::Dark ? "#FF6666" : "#AA0000") + ";\">" + issue.shortissueDescription() + "</td></tr></table></html>");
    }
}

void dlgFilePreview::on_btnOpenFile_clicked() {
    QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(_filePath.replace("\\", "/"))));
}
