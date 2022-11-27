#include "wrneditor.h"
#include "ui_wrneditor.h"

wRNEditor::wRNEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wRNEditor)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    resize(misc.sizeWindow(0.5, 0.75));
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - Release Notes Editor");

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());


    qInfo().noquote() << objectName() + " started.";
}

wRNEditor::~wRNEditor()
{
    delete ui;
}

void wRNEditor::on_btnLoad_clicked()
{
    ui->twgJsonSelection->clear();
    ui->twgJsonOthers->clear();

    QClipboard* clipboard = QApplication::clipboard();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(clipboard->text().toUtf8());

    QJsonArray selection = jsonDoc["selection"].toArray();

    int i = 1;

    foreach (QJsonValue value, selection)
    {
        QJsonObject obj = value.toObject();
        ui->twgJsonSelection->insertRow(ui->twgJsonSelection->rowCount());
        ui->twgJsonSelection->item(i, 1)->setText(obj["en"].toString());
        ui->twgJsonSelection->item(i, 2)->setText(obj["de"].toString());
        i++;
    }

    QJsonArray others = jsonDoc["others"].toArray();

    i = 1;
    foreach (QJsonValue value, others)
    {
        QJsonObject obj = value.toObject();
        ui->twgJsonOthers->insertRow(ui->twgJsonSelection->rowCount());
        ui->twgJsonOthers->item(i, 1)->setText(obj["en"].toString());
        ui->twgJsonOthers->item(i, 2)->setText(obj["de"].toString());
        i++;
    }

}

void wRNEditor::on_btnSave_clicked()
{
    QString json = "{ \"selection\": [";
    for (int i = 0; i < ui->twgJsonSelection->rowCount(); i++)
    {
        if (i != 0)
            json += ",";

        json += "{ \"en\": \"";
        json += ui->twgJsonSelection->item(i, 0)->text();
        json += "\", \"de\": \"";
        json += ui->twgJsonSelection->item(i, 1)->text();
        json += "\" }";
    }

    json += "], \"others\": [";

    for (int i = 0; i < ui->twgJsonOthers->rowCount(); i++)
    {
        if (i != 0)
            json += ",";

        json += "{ \"en\": \"";
        json += ui->twgJsonOthers->item(i, 0)->text();
        json += "\", \"de\": \"";
        json += ui->twgJsonOthers->item(i, 1)->text();
        json += "\" }";
    }

    json += "]}";

    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(jsonDoc.toJson());
}

void wRNEditor::on_btnAddSelection_clicked()
{
    ui->twgJsonSelection->insertRow(ui->twgJsonSelection->rowCount());
}

void wRNEditor::on_btnRemoveSelection_clicked()
{
    ui->twgJsonSelection->removeRow(ui->twgJsonSelection->currentRow());
}

void wRNEditor::on_btnAddOthers_clicked()
{
    ui->twgJsonOthers->insertRow(ui->twgJsonOthers->rowCount());
}

void wRNEditor::on_btnRemoveOthers_clicked()
{
    ui->twgJsonOthers->removeRow(ui->twgJsonOthers->currentRow());
}
