#ifndef WGLOBALPROPS_H
#define WGLOBALPROPS_H


#include <QMainWindow>
#include "OTBackend/OTOmsiFileHandler.h"
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OCC.h"
#include "OTWidgets/input3dcoordinates.h"
#include "OTModules/wMaps/wGlobalProps/weditgroundtexture.h"
#include "OTModules/wMaps/wGlobalProps/weditseason.h"

namespace Ui {
class wGlobalProps;
}

class wGlobalProps : public QMainWindow
{
    Q_OBJECT

public:
    explicit wGlobalProps(OCMap::Global globalProps, QWidget *parent = nullptr);
    ~wGlobalProps();

signals:
    void returnGlobalProps(OCMap::Global globalProps);

private slots:
    void on_btnEditProtectedProps_clicked();

    void on_btnSave_clicked();

    void on_btnBISelectPicturePath_clicked();

    void recieveGroundTexture(OCMap::Global::Texture texture, int index);

    void on_ledFriendlyname_textEdited(const QString &arg1);

    void on_ledName_textEdited(const QString &arg1);

    void on_btnSyncNames_clicked(bool checked);

    void on_cuwGroundTextures_addClicked();

    void on_cuwGroundTextures_editClicked();

    void on_cuwGroundTextures_removeClicked();

    void on_btnTest_clicked();

    void on_cobxCurrency_currentIndexChanged(int index);

    void on_cobxTicketpack_currentIndexChanged(int index);

    void on_pteDescription_textChanged();

    void on_cbxWorldCoordinates_stateChanged(int arg1);

    void on_cbxDynamicHelpArrows_stateChanged(int arg1);

    void on_cbxLeftHandDriving_stateChanged(int arg1);

    void on_cbxRealrail_stateChanged(int arg1);

    void on_tieRepairTime_userTimeChanged(const QTime &time);

    void on_sbxRealYearOffset_valueChanged(int arg1);

    void on_cbxBackgroundImage_stateChanged(int arg1);

    void on_cuwBIPictureSize_name1Changed(float value);

    void on_cuwYears_name1Changed(float value);

    void on_cuwYears_name2Changed(float value);

    void on_cuwBIPictureSize_name2Changed(float value);

    void on_sbxBIStartWidth_valueChanged(int arg1);

    void on_sbxBIStartHeight_valueChanged(int arg1);

    void on_cuwDVTilePosition_name1Changed(float value);

    void on_cuwDVTilePosition_name2Changed(float value);

    void on_cuwDVPosition_name1Changed(float value);

    void on_cuwDVPosition_name2Changed(float value);

    void on_cuwDVPosition_name3Changed(float value);

    void on_cuwDVRotation_name1Changed(float value);

    void on_cuwDVRotation_name2Changed(float value);

    void on_sbxDVDistanceFromPosition_valueChanged(int arg1);

    void on_cuwSeasons_addClicked();

    void on_cuwSeasons_editClicked();

    void on_cuwSeasons_removeClicked();

    void on_actionClose_triggered();

private:
    Ui::wGlobalProps *ui;
    OTSettings set;
    wEditGroundTexture *WEDITGROUNDTEXTURE;
    wEditSeason *WEDITSEASON;
    OTOMSIFileHandler filehandler;

    void reloadUI();

    bool setupFinished;

    OCMap::Global props;

    QStandardItemModel *modelGroundTextures;
    QStandardItemModel *modelSeasons;
    QStandardItemModel *modelVehicleDensity;
    QStandardItemModel *modelPassengerDensity;
    QStandardItemModel *modelEntrypoints;
    QStandardItemModel *modelTiles;

    QList<QPair<QString, QString>> currency;
    QList<QPair<QString, QString>> tickets;
};

#endif // WGLOBALPROPS_H
