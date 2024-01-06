#include "wplaceobjects.h"
#include "ui_wplaceobjects.h"

wPlaceObjects::wPlaceObjects(OCMap::Global globalProps, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wPlaceObjects)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(tr("Place objects by terrain layer painting"));

    ui->cuwObjects->setEditEnabled(false);
    checkObjectList();

    props = globalProps;

    loadUi();

    ui->sbxTerrainLayerID->setMaximum(props.groundTextures.count() - 1);
    on_sbxTerrainLayerID_valueChanged(ui->sbxTerrainLayerID->value());
    on_hslObjectDensity_sliderMoved(ui->hslObjectDensity->value());

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    qInfo().noquote() << objectName() + " started";
}

wPlaceObjects::~wPlaceObjects()
{
    delete ui;
}

void wPlaceObjects::on_cuwObjects_addClicked()
{
    QStringList objects = QFileDialog::getOpenFileNames(this, tr("Select objects"), set.read("main", "mainDir").toString() + "/Sceneryobjects", tr("OMSI Sceneryobject") + " (*.sco)");

    foreach (QString current, objects)
        ui->lwgObjects->addItem(current.remove(set.read("main", "mainDir").toString() + "/"));

    checkObjectList();
}

void wPlaceObjects::on_cuwObjects_removeClicked()
{
    qDeleteAll(ui->lwgObjects->selectedItems());
    checkObjectList();
}

void wPlaceObjects::checkObjectList()
{
    ui->cuwObjects->setRemoveEnabled(ui->lwgObjects->count() != 0);
}

void wPlaceObjects::loadUi()
{
    foreach (OCMap::Global::TileInformation current, props.tiles)
    {
        QListWidgetItem *item = new QListWidgetItem(QString("%1 / %2\t%3").arg(QString::number(current.position.x), QString::number(current.position.y), current.filename));
        item->setCheckState(Qt::Checked);
        ui->lwgTiles->addItem(item);
    }
}

void wPlaceObjects::on_hslObjectDensity_sliderMoved(int position)
{
    ui->sbxObjectDensity->setValue(position);
}

void wPlaceObjects::on_sbxTerrainLayerID_valueChanged(int arg1)
{
    ui->lblLayerTexture->setText(props.groundTextures[arg1].mainTex);

    if (props.groundTextures[arg1].mainTex.endsWith(".dds"))
    {
        texconv.convert("bmp", set.read("main", "mainDir").toString() + "/" + props.groundTextures[arg1].mainTex, convertedPreviewImage);
        test = QPixmap(convertedPreviewImage.fileName());
        ui->lblLayerTexturePicture->setPixmap(test);
        qInfo() << convertedPreviewImage.fileName();
    }
    else
    {
        ui->lblLayerTexturePicture->setPixmap(QPixmap(set.read("main", "mainDir").toString() + "/" + props.groundTextures[arg1].mainTex));
    }
}

/*
        QTemporaryFile bmpFile;
        bmpFile.open();
        texconv.convert("bmp", props.filepath.remove("global.cfg") + "texture/map/" + "tile_0_0.map.1.dds", QDir::tempPath());
        ui->lblLayerTexturePicture->setPixmap(QPixmap().fromImage(QImage(QDir::tempPath() + "/" + "tile_0_0.map.1.bmp")));

        QFile(QDir::tempPath() + "/" + "tile_0_0.map.1.bmp").remove();
*/

void wPlaceObjects::on_btnStart_clicked()
{
    enableUi(false);
    for (int i = 0; i < ui->lwgTiles->count(); i++)
    {
        if (ui->lwgTiles->item(i)->checkState() == Qt::Checked)
        {
            QTemporaryFile layerSource;

            QString layerName = props.tiles[i].filename + "." + QString::number(ui->sbxTerrainLayerID->value()) + ".dds";

            QString originalFilename = props.filepath.remove("global.cfg") + "texture/map/" + layerName;

            texconv.convert("bmp", props.filepath.remove("global.cfg") + "texture/map/" + layerName, layerSource);

            QImage layer(layerSource.fileName());
            qInfo() << layerSource.fileName();

            placeObjectsFromLayer(layer);

            if (ui->cbxClearLayer->isChecked()) QFile(originalFilename).remove(); // TOD: Warnung


        }
    }

    enableUi(true);
}
void wPlaceObjects::placeObjectsFromLayer(QImage &image)
{
    if (image.isNull()) return;
    int width = image.width();
    int height = image.height();

    QList<QPoint> blackPixels;

    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            if (image.pixelColor(x, y) == Qt::white)
                blackPixels << QPoint(x, y);

    float currentDensity = QVariant(ui->hslObjectDensity->value()).toFloat() / 100;
    float maxDensity = QVariant(ui->hslObjectDensity->maximum()).toFloat() / 100;

    int anzZPObj = blackPixels.count() * (ui->dsbxTileSize->value() / width) * ((currentDensity <= 1) ? 1 : maxDensity);

    QString bla;

    for (int i = 0; i < anzZPObj; i++)
    {
        if (QRandomGenerator::global()->generateDouble() < (QVariant(currentDensity).toFloat() / ((currentDensity <= 1) ? 1 : maxDensity)))
        {
            int index = QRandomGenerator::global()->bounded(0, blackPixels.count());

            float xVariance = (QRandomGenerator::global()->bounded(-100, 100) * ((ui->dsbxTileSize->value() / width) / 2)) / 100.0;
            float yVariance = (QRandomGenerator::global()->bounded(-100, 100) * ((ui->dsbxTileSize->value() / width) / 2)) / 100.0;

            float x = (blackPixels[index].x() * (ui->dsbxTileSize->value() / width)) + xVariance;
            float y = (blackPixels[index].y() * (ui->dsbxTileSize->value() / width)) + yVariance; // jaaaaa, ich weiss....

            y = ui->dsbxTileSize->value() - y;

            bla += QString("[object]\r\n"
                           "0\r\n"
                           "Sceneryobjects\\Oberpfalz 3D\\Krummenaab2\\BigMac.sco\r\n"
                           "3405\r\n"
                           "%1\r\n"
                           "%2\r\n"
                           "0\r\n"
                           "%3\r\n"
                           "0\r\n"
                           "0\r\n"
                           "4\r\n"
                           "Gras.tga\r\n"
                           "0.42\r\n"
                           "4.71\r\n"
                           "\r\n").arg(QString::number(x),
                            QString::number(y),
                            QString::number(QRandomGenerator::global()->bounded(0, 36000) / 100.0));
        }
    }

    misc.copy(bla);
}

void wPlaceObjects::enableUi(bool enable)
{
    ui->centralwidget->setEnabled(enable);
}

void wPlaceObjects::on_actionClose_triggered()
{
    close();
}

void wPlaceObjects::on_sbxObjectDensity_valueChanged(int arg1)
{
    ui->hslObjectDensity->setValue(arg1);
}
