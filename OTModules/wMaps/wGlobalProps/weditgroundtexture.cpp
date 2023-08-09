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

    setWindowTitle(tr("Edit ground texture"));

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    texture = tex;
    textureIndex = index;

    reloadUI();

    qInfo().noquote() << objectName() + " started";
}

wEditGroundTexture::~wEditGroundTexture()
{
    delete ui;
}

void wEditGroundTexture::reloadUI()
{
    // TODO
}

void wEditGroundTexture::saveUI()
{
    // TODO
}

void wEditGroundTexture::on_btnMainTex_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select main texture"), (ui->ledMainTex->text().isEmpty()) ? set.read("main", "mainDir").toString() + "/Texture" : set.read("main", "mainDir").toString() + "/" + ui->ledMainTex->text(), OTStrings::textureSuffixes);

    if (!path.isEmpty()) {
        texture.mainTex = path.remove(set.read("main", "mainDir").toString());
        reloadUI();
    }
}

void wEditGroundTexture::on_btnSubTex_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select detail texture"), (ui->ledSubTex->text().isEmpty()) ? set.read("main", "mainDir").toString() + "/Texture" : set.read("main", "mainDir").toString() + "/" + ui->ledSubTex->text(), OTStrings::textureSuffixes);

    if (!path.isEmpty()) {
        texture.subTex = path.remove(set.read("main", "mainDir").toString());
        reloadUI();
    }
}

void wEditGroundTexture::on_btnSave_clicked()
{
    returnTexture(texture, textureIndex);
    close();
}


void wEditGroundTexture::on_btnClose_clicked()
{
    close();
}

