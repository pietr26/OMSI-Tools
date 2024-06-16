#include "wdggeneral.h"
#include "ui_wdggeneral.h"

wdgGeneral::wdgGeneral(QWidget *parent, OTFontModel *font)
    : QWidget(parent)
    , ui(new Ui::wdgGeneral),
    _font(font)
{
    ui->setupUi(this);

    ui->sbxMaxHeigthOfChars->clear();
    ui->sbxDistanceBetweenChars->clear();

    checkPropValidity();
}

wdgGeneral::~wdgGeneral()
{
    delete ui;
}

void wdgGeneral::checkPropValidity()
{
    ui->lblFontName->setStyleSheet("");
    ui->lblColorTexture->setStyleSheet("");
    ui->lblAlphaTexture->setStyleSheet("");
    ui->lblMaxHeigthOfChars->setStyleSheet("");
    ui->lblDistanceBetweenChars->setStyleSheet("");

    if (_font->name == "")
        ui->lblFontName->setStyleSheet("color:red");

    if ((_font->colorTexture != "") && !QFile(set.read("main", "mainDir").toString() + "/Fonts/" + _font->colorTexture).exists())
        ui->lblColorTexture->setStyleSheet("color:red");

    if (_font->alphaTexture == "" || !QFile(set.read("main", "mainDir").toString() + "/Fonts/" + _font->alphaTexture).exists())
        ui->lblAlphaTexture->setStyleSheet("color:red");

    if (ui->sbxMaxHeigthOfChars->text() == "" || (_font->maxHeightOfChars == 0))
        ui->lblMaxHeigthOfChars->setStyleSheet("color:red");

    if (ui->sbxDistanceBetweenChars->text() == "")
        ui->lblDistanceBetweenChars->setStyleSheet("color:red");

    if (QFile(set.read("main", "mainDir").toString() + "/Fonts/" + _font->alphaTexture).exists())
    {
        QImage alphaTexture(set.read("main", "mainDir").toString() + "/Fonts/" + _font->alphaTexture);

        if (alphaTexture.width() != 0 || alphaTexture.height() != 0)
        {
            if (_font->maxHeightOfChars > QString::number(alphaTexture.height()).toInt())
                ui->lblMaxHeigthOfChars->setStyleSheet("color:red");

            if (_font->distanceBetweenChars > QString::number(alphaTexture.width()).toInt())
                ui->lblDistanceBetweenChars->setStyleSheet("color:red");
        }
    }
}

void wdgGeneral::on_ledFontName_textChanged(const QString &arg1)
{
    _font->name = arg1;
    checkPropValidity();
    emit checkCharValidity();
    emit setModified(true);
}

void wdgGeneral::on_ledColorTexture_textChanged(const QString &arg1)
{
    _font->colorTexture = arg1;
    checkPropValidity();
    emit checkCharValidity();
    emit setModified(true);

    //loadTexPreview();
}


void wdgGeneral::on_ledAlphaTexture_textChanged(const QString &arg1)
{
    _font->alphaTexture = arg1;
    checkPropValidity();
    emit checkCharValidity();
    emit setModified(true);

    //loadTexPreview();
}

void wdgGeneral::on_btnColorTexture_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select color texture..."), set.read("main", "mainDir").toString() + "/Fonts", tr("Bitmap picture") + " (*.bmp)");

    if (filename == "")
        return;

    _font->colorTexture = filename.remove(0, QString(set.read("main", "mainDir").toString() + "/Fonts").size() + 1);

    if (ui->ledColorTexture->text() != _font->colorTexture)
    {
        qDebug() << QString("New color texture: '%1'").arg(filename);
        emit setModified(true);
    }

    ui->ledColorTexture->setText(_font->colorTexture);
}

void wdgGeneral::on_btnAlphaTexture_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select alpha texture..."), set.read("main", "mainDir").toString() + "/Fonts", tr("Bitmap picture") + " (*.bmp)");
    if (filename == "")
        return;

    _font->alphaTexture = filename.remove(0, QString(set.read("main", "mainDir").toString() + "/Fonts").size() + 1);

    if (ui->ledAlphaTexture->text() != _font->alphaTexture)
    {
        qDebug() << QString("New alpha texture: '%1'").arg(filename);
        emit setModified(true);
    }

    ui->ledAlphaTexture->setText(_font->alphaTexture);
}

void wdgGeneral::on_sbxMaxHeigthOfChars_textChanged(const QString &arg1)
{
    _font->maxHeightOfChars = arg1.toInt();
    checkPropValidity();
    emit checkCharValidity();
    emit setModified(true);
}

void wdgGeneral::on_sbxDistanceBetweenChars_textChanged(const QString &arg1)
{
    _font->distanceBetweenChars = arg1.toInt();
    checkPropValidity();
    emit checkCharValidity();
    emit setModified(true);
}

void wdgGeneral::reloadUi()
{
    ui->ledFontName->setText(_font->name);
    ui->ledColorTexture->setText(_font->colorTexture);
    ui->ledAlphaTexture->setText(_font->alphaTexture);
    if (_font->maxHeightOfChars == -1) ui->sbxMaxHeigthOfChars->clear(); else ui->sbxMaxHeigthOfChars->setValue(_font->maxHeightOfChars);
    if (_font->distanceBetweenChars == -1) ui->sbxDistanceBetweenChars->clear(); else ui->sbxDistanceBetweenChars->setValue(_font->distanceBetweenChars);

    checkPropValidity();
}
