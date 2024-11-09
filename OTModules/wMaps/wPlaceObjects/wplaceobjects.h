#ifndef WPLACEOBJECTS_H
#define WPLACEOBJECTS_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTExternal.h"
#include "OTBackend/OCC/OCMap.h"

namespace Ui {
class wPlaceObjects;
}

class wPlaceObjects : public QMainWindow
{
    Q_OBJECT

public:
    explicit wPlaceObjects(OCMap::Map::Global globalProps, QWidget *parent = nullptr);
    ~wPlaceObjects();

private slots:
    void on_cuwObjects_addClicked();

    void on_cuwObjects_removeClicked();

    void on_hslObjectDensity_sliderMoved(int position);

    void on_sbxTerrainLayerID_valueChanged(int arg1);

    void on_btnStart_clicked();

    void on_actionClose_triggered();

    void on_cuwZVariance_name1Changed(float );

    void on_cuwZVariance_name2Changed(float );

    void on_sbxTerrainLayerID_textChanged(const QString &arg1);

    void on_actionPresetGrass_triggered();

    void on_tbnObjectPresets_clicked();

    void on_btnSave_clicked();

    void on_btnTilesAll_clicked();

    void on_btnTilesNone_clicked();

    void on_sbxObjectDensity_valueChanged(int arg1);

signals:
    void returnGlobalProps(OCMap::Map::Global props);

private:
    Ui::wPlaceObjects *ui;
    OTSettings set;
    OTDirectXTexConv texconv;

    OCMap::Map map;

    QTemporaryFile convertedPreviewImage;

    QPixmap test;

    void checkObjectList();

    void loadUi();

    QString placeObjectsFromLayer(QImage &image);

    void enableUi(bool enable);

    QStringList objectEntries;

    void checkStartEnabled();
};

#endif // WPLACEOBJECTS_H
