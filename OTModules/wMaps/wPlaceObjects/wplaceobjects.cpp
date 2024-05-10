#include "wplaceobjects.h"
#include "ui_wplaceobjects.h"

#include <QPainter>

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

    ui->cuwZVariance->setValue1(0);
    ui->cuwZVariance->setValue2(0);

    ui->sbxTerrainLayerID->setMaximum(props.groundTextures.count() - 1);
    on_sbxTerrainLayerID_valueChanged(ui->sbxTerrainLayerID->value());
    on_hslObjectDensity_sliderMoved(ui->hslObjectDensity->value());

    checkStartEnabled();

    ui->tbnObjectPresets->addAction(ui->actionPresetGrass);

    ui->statusbar->addPermanentWidget(ui->pgbProgress);
    ui->pgbProgress->setVisible(false);

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
        ui->lwgObjects->addItem(current.remove(set.read("main", "mainDir").toString() + "/").replace("/", "\\"));

    checkObjectList();
    checkStartEnabled();
}

void wPlaceObjects::on_cuwObjects_removeClicked()
{
    qDeleteAll(ui->lwgObjects->selectedItems());
    checkObjectList();
    checkStartEnabled();
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
        item->setCheckState(Qt::Unchecked);
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

void wPlaceObjects::on_btnStart_clicked()
{
    qInfo() << "Start object placing";
    enableUi(false);

    QStringList tiles;

    for (int i = 0; i < ui->lwgTiles->count(); i++)
        if (ui->lwgTiles->item(i)->checkState() == Qt::Checked)
            tiles << ui->lwgTiles->item(i)->text();


    qInfo() << "Get objects data...";
    for (int i = 0; i < ui->lwgObjects->count(); i++)
    {
        QString labels;

        QFile sco(set.read("main", "mainDir").toString() + "/" + ui->lwgObjects->item(i)->text());

        if (sco.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&sco);

            bool isTree = false;

            while (!in.atEnd())
            {
                if (in.readLine() == "[tree]")
                {
                    isTree = true;
                    QString texture = in.readLine();
                    QString minHeight = in.readLine();
                    QString maxHeight = in.readLine();
                    QString minRatio = in.readLine();
                    QString maxRatio = in.readLine();

                    double height = QRandomGenerator::global()->bounded((QString::number(minHeight.toDouble() * 100)).toInt(), QString::number(maxHeight.toDouble() * 100).toInt()) / 100.00;
                    double ratio = QRandomGenerator::global()->bounded((QString::number(minRatio.toDouble() * 100)).toInt(), QString::number(maxRatio.toDouble() * 100).toInt()) / 100.0;

                    labels = "3\n" +
                             texture + "\n" +
                             QString::number(height) + "\n" +
                             QString::number(ratio) + "\n\n";
                }
            }

            sco.close();

            if (!isTree) labels = "0\n\n";

            objectEntries << QString("[object]\n"
                                     "0\n" // ?
                                     "%1\n" // Path
                                     "%2\n" // globalID
                                     "%3\n" // xPos
                                     "%4\n" // yPos
                                     "%5\n" // zPos
                                     "%6\n" // rot
                                     "0\n" // ???
                                     "0\n" // ???
                                     + labels).arg(ui->lwgObjects->item(i)->text().replace("/", "\\"), "%1", "%2", "%3", "%4", "%5");
        }
        else
        {
            qCritical() << QString("Could not open file %1!").arg(sco.fileName());
        }
    }

    qInfo() << "Start object placing";

    for (int i = 0; i < ui->lwgTiles->count(); i++)
    {
        if (ui->lwgTiles->item(i)->checkState() == Qt::Checked)
        {
            qInfo() << "Checked tile:" << props.tiles[i].filename;
            ui->statusbar->showMessage("Tile: " + props.tiles[i].filename);

            QTemporaryFile layerSource;

            QString layerName = props.tiles[i].filename + "." + QString::number(ui->sbxTerrainLayerID->value()) + ".dds";

            QString originalFilename = QString(props.filepath).remove("global.cfg") + "texture/map/" + layerName;

            texconv.convert("bmp", QString(props.filepath).remove("global.cfg") + "texture/map/" + layerName, layerSource);

            QImage layer(layerSource.fileName());

            QString newObjectEntries = placeObjectsFromLayer(layer);

            newObjectEntries.replace("Ä", "Ae", Qt::CaseSensitive);
            newObjectEntries.replace("Ö", "Oe", Qt::CaseSensitive);
            newObjectEntries.replace("Ü", "Ue", Qt::CaseSensitive);
            newObjectEntries.replace("ä", "ae", Qt::CaseSensitive);
            newObjectEntries.replace("ö", "oe", Qt::CaseSensitive);
            newObjectEntries.replace("ü", "ue", Qt::CaseSensitive);

            if (ui->cbxClearLayer->isChecked()) QFile(originalFilename).remove(); // TODO: Warnung

            if (!ui->cbxDryRun->isChecked())
            {
                // Change map file
                if (!QDir().exists(QString(props.filepath).remove("global.cfg") + "/backup")) qDebug() << "Backup dir create:" << QDir().mkdir(QString(props.filepath).remove("global.cfg") + "/backup");
                if (QFile(QString(props.filepath).remove("global.cfg") + "/backup/" + props.tiles[i].filename).exists()) QFile(QString(props.filepath).remove("global.cfg") + "/backup/" + props.tiles[i].filename).remove();
                QFile::copy(QString(props.filepath).remove("global.cfg") + "/" + props.tiles[i].filename, QString(props.filepath).remove("global.cfg") + "/backup/" + props.tiles[i].filename);

                QFile tile(QString(props.filepath).remove("global.cfg") + "/" + props.tiles[i].filename);

                if (tile.open(QFile::WriteOnly | QFile::Text | QFile::Append))
                {
                    QTextStream out(&tile);
                    out.setEncoding(QStringConverter::Utf16LE);

                    out << "\n\n";
                    out << newObjectEntries;

                    tile.close();
                }
            }
            else qInfo() << "DRY RUN - tile data will NOT be written!";

            qApp->processEvents();
        }
    }

    props.nextIDCode++;

    enableUi(true);

    qInfo() << "Finished. New nextIDCode:" << props.nextIDCode;
    ui->statusbar->showMessage("Finished. New nextIDCode: " + QString::number(props.nextIDCode) + " - nextIDCode still not saved (see button)!");
}

QString wPlaceObjects::placeObjectsFromLayer(QImage &image)
{
    if (image.isNull()) return "";
    int width = image.width();
    int height = image.height();

    quint64 pixelCount = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (image.pixelColor(x, y) == Qt::white) {
                image.setPixelColor(x, y, Qt::gray);
                pixelCount++;
            } else {
                image.setPixelColor(x, y, Qt::lightGray);
            }
        }
    }

    QImage largeImage = image.scaled(QSize(ui->dsbxTileSize->value(), ui->dsbxTileSize->value()), Qt::IgnoreAspectRatio, Qt::FastTransformation);

    QPainter painter(&largeImage);
    painter.setBrush(QColor(0, 0, 255, 64));

    // determine target densisty
    int densisty = ui->sbxObjectDensity->value(); // ( n / m^2)

    // pixel size
    float pixelWidth = ui->dsbxTileSize->value() / image.width();

    // determine size of the "allowed" area in m^2
    float areaSize = pow(pixelWidth, 2) * pixelCount / 10000.0; // ( m / res * n  ^ 2 = m^2 ) / convert to ha

    // determine needed amount of objects
    int   placedObjectsCount = areaSize * densisty; // ( m^2 * n/m^2 = n )

    QLabel *imageLabel = new QLabel(nullptr);
    imageLabel->setPixmap(QPixmap::fromImage(largeImage));
    QVBoxLayout layout;
    layout.addWidget(imageLabel);
    QWidget *window = new QWidget(nullptr);
    window->setWindowTitle(tr("Placing preview"));
    window->setLayout(&layout);
    if(ui->cbxEnablePreview->isChecked())
        window->show();

    QString result;

    ui->pgbProgress->setMaximum(placedObjectsCount);
    ui->pgbProgress->setValue(0);
    ui->pgbProgress->setVisible(true);

    for (int i = 0; i < placedObjectsCount; i++)
    {
        props.nextIDCode++;
        QList<QPoint> locations;

        for(int y = 0; y < largeImage.height(); y++)
            for(int x = 0; x < largeImage.width(); x++)
                if (largeImage.pixelColor(x, y) == Qt::gray)
                    locations << QPoint(x, y);

        if(locations.empty())
            break;

        int index = QRandomGenerator::global()->bounded(0, locations.count());
        QPoint placePoint = locations[index];
        float minimumDistance = ui->dsbxMinimumDistance->value();
        painter.drawEllipse(placePoint.x() - minimumDistance,
                            placePoint.y() - minimumDistance,
                            minimumDistance * 2,
                            minimumDistance * 2);

        largeImage.setPixelColor(placePoint, QColor(255, 0, 0, 255));
        imageLabel->setPixmap(QPixmap::fromImage(largeImage));

        float x = placePoint.x();
        float y = placePoint.y();
        y = ui->dsbxTileSize->value() - y; // mirror y (because of different coordinate systems between omsi tiles and pixel coordinates of QPixmap
        float z;

        if (ui->cuwZVariance->getValue1() == ui->cuwZVariance->getValue2())
            z = ui->cuwZVariance->getValue1();
        else
            z = QVariant(QRandomGenerator::global()->bounded(QVariant(ui->cuwZVariance->getValue1() * 100).toInt(),
                                                        QVariant(ui->cuwZVariance->getValue2() * 100).toInt())
                                                        ).toFloat() / 100;

        result += objectEntries[QRandomGenerator::global()->bounded(0, objectEntries.count())].arg(QString::number(props.nextIDCode),
                                                                                                QString::number(x),
                                                                                                QString::number(y),
                                                                                                QString::number(z),
                                                                                                QString::number(QRandomGenerator::global()->bounded(0, 36000) / 100.0));

        if(i % 10 == 0)
        {
            ui->pgbProgress->setValue(i + 1);
            qApp->processEvents();
        }
    }

    ui->pgbProgress->setVisible(false);

    return result;
}

void wPlaceObjects::enableUi(bool enable)
{
    ui->centralwidget->setEnabled(enable);
}

void wPlaceObjects::checkStartEnabled()
{
    bool enabled = (ui->cuwZVariance->getValue1() <= ui->cuwZVariance->getValue2()) &&
                   (ui->sbxTerrainLayerID->value() != 0) &&
                   (ui->lwgObjects->count() != 0);
    ui->btnStart->setEnabled(enabled);
}

void wPlaceObjects::on_actionClose_triggered()
{
    close();
}

void wPlaceObjects::on_cuwZVariance_name1Changed(float value)
{
    Q_UNUSED(value);
    checkStartEnabled();
}

void wPlaceObjects::on_cuwZVariance_name2Changed(float value)
{
    Q_UNUSED(value);
    checkStartEnabled();
}

void wPlaceObjects::on_sbxTerrainLayerID_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    checkStartEnabled();
}

void wPlaceObjects::on_actionPresetGrass_triggered()
{
    ui->sbxObjectDensity->setValue(8000);

    ui->lwgObjects->addItem("Sceneryobjects\\Oberpfalz 3D\\Krummenaab\\Gras.sco");
    ui->lwgObjects->addItem("Sceneryobjects\\Oberpfalz 3D\\Krummenaab\\Gras_2.sco");
    ui->lwgObjects->addItem("Sceneryobjects\\Oberpfalz 3D\\Krummenaab\\Gras_3.sco");
    ui->lwgObjects->addItem("Sceneryobjects\\Oberpfalz 3D\\Krummenaab\\Gras_4.sco");

    checkObjectList();
}

void wPlaceObjects::on_tbnObjectPresets_clicked()
{
    ui->tbnObjectPresets->showMenu();
}

void wPlaceObjects::on_btnSave_clicked()
{
    emit returnGlobalProps(props);
    close();
}

void wPlaceObjects::on_btnTilesAll_clicked()
{
    for (int i = 0; i < ui->lwgTiles->count(); i++) ui->lwgTiles->item(i)->setCheckState(Qt::Checked);
}

void wPlaceObjects::on_btnTilesNone_clicked()
{
    for (int i = 0; i < ui->lwgTiles->count(); i++) ui->lwgTiles->item(i)->setCheckState(Qt::Unchecked);
}

void wPlaceObjects::on_sbxObjectDensity_valueChanged(int arg1)
{
    ui->hslObjectDensity->setValue(arg1);
}
