#ifndef WGLOBALPROPS_H
#define WGLOBALPROPS_H

#include <QMainWindow>
#include <OTBackend/OTGlobal.h>
#include <OTBackend/OCC.h>
#include <OTWidgets/input3dcoordinates.h>
#include <OTModules/wMaps/wGlobalProps/weditgroundtexture.h>

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

    void on_cbxBackgroundImage_stateChanged(int arg1);

    void on_cbxDefaultView_stateChanged(int arg1);

    void recieveGroundTexture(OCMap::Global::Texture texture, int index);

    void on_btnGTAdd_clicked();

    void on_btnGTEdit_clicked();

    void on_btnGTRemove_clicked();

private:
    Ui::wGlobalProps *ui;
    OTSettings set;
    wEditGroundTexture *WEDITGROUNDTEXTURE;

    void reloadUI();
    void saveUI();

    OCMap::Global props;
};

#endif // WGLOBALPROPS_H
