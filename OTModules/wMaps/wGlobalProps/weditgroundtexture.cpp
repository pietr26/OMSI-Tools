#include "weditgroundtexture.h"
#include "ui_weditgroundtexture.h"

wEditGroundTexture::wEditGroundTexture(OCMap::Global::Texture tex, int index, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wEditGroundTexture)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(tr("Edit terrain texture"));

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    textureIndex = index;
    if (textureIndex != -1) texture = tex;


    reloadUI();

    qInfo().noquote() << objectName() + " started";
}

wEditGroundTexture::~wEditGroundTexture()
{
    delete ui;
}

void wEditGroundTexture::reloadUI()
{
    ui->ledMainTex->setText(texture.mainTex);
    ui->ledSubTex->setText(texture.subTex);

    ui->sbxMainTexRepeating->setValue(texture.mainTexRepeating);
    ui->sbxSubTexRepeating->setValue(texture.subTexRepeating);

    ui->cbxTexSizeExp->setCurrentIndex(texture.texSizeExponent);
}

void wEditGroundTexture::on_btnMainTex_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select main texture"), (texture.mainTex.isEmpty()) ? set.read("main", "mainDir").toString() + "/Texture" : set.read("main", "mainDir").toString() + "/" + texture.mainTex, OTStrings::textureSuffixes);

    if (!path.isEmpty()) {
        texture.mainTex = path.remove(set.read("main", "mainDir").toString());
        reloadUI();
    }
}

void wEditGroundTexture::on_btnSubTex_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select detail texture"), (texture.subTex.isEmpty()) ? set.read("main", "mainDir").toString() + "/Texture" : set.read("main", "mainDir").toString() + "/" + texture.subTex, OTStrings::textureSuffixes);

    if (!path.isEmpty()) {
        texture.subTex = path.remove(set.read("main", "mainDir").toString());
        reloadUI();
    }
}

void wEditGroundTexture::on_btnSave_clicked()
{
    emit returnTexture(texture, textureIndex);
    close();
}

void wEditGroundTexture::on_btnClose_clicked()
{
    close();
}

void wEditGroundTexture::on_sbxMainTexRepeating_valueChanged(int arg1)
{
    texture.mainTexRepeating = arg1;
}

void wEditGroundTexture::on_sbxSubTexRepeating_valueChanged(int arg1)
{
    texture.subTexRepeating = arg1;
}

void wEditGroundTexture::on_cbxTexSizeExp_currentIndexChanged(int index)
{
    texture.texSizeExponent = index;
}
