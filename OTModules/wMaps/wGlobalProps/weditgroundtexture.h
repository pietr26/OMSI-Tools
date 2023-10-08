#ifndef WEDITGROUNDTEXTURE_H
#define WEDITGROUNDTEXTURE_H

#include <QMainWindow>
#include <OTBackend/OCC.h>
#include <OTBackend/OTGlobal.h>

namespace Ui {
class wEditGroundTexture;
}

class wEditGroundTexture : public QMainWindow
{
    Q_OBJECT

public:
    explicit wEditGroundTexture(OCMap::Global::Texture tex, int index, QWidget *parent = nullptr);
    ~wEditGroundTexture();

signals:
    void returnTexture(OCMap::Global::Texture texture, int index);

private slots:
    void on_btnMainTex_clicked();

    void on_btnSubTex_clicked();

    void on_btnSave_clicked();

    void on_btnClose_clicked();

    void on_sbxMainTexRepeating_valueChanged(int arg1);

    void on_sbxSubTexRepeating_valueChanged(int arg1);

    void on_cbxTexSizeExp_currentIndexChanged(int index);

private:
    Ui::wEditGroundTexture *ui;
    OTSettings set;

    void reloadUI();

    OCMap::Global::Texture texture = OCMap::Global::Texture();
    int textureIndex;
};

#endif // WEDITGROUNDTEXTURE_H
