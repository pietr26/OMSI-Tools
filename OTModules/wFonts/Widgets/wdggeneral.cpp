#include "wdggeneral.h"
#include "ui_wdggeneral.h"

wdgGeneral::wdgGeneral(QWidget *parent, OCFont *font)
    : QWidget(parent)
    , ui(new Ui::wdgGeneral),
    _font(font)
{
    ui->setupUi(this);

    checkPropValidity();

    ui->cobxCurrentFont->addItem("0");
    ui->cobxCurrentFont->addItem("1");

    setupFinished = true;
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

    if (_font->fonts[currentFontIndex].name().isEmpty())
        ui->lblFontName->setStyleSheet("color:red");

    if (!_font->fonts[currentFontIndex].colorTexture().isEmpty() && !QFile(set.read("main", "mainDir").toString() + "/Fonts/" + _font->fonts[currentFontIndex].colorTexture()).exists())
        ui->lblColorTexture->setStyleSheet("color:red");

    if (_font->fonts[currentFontIndex].alphaTexture().isEmpty() || !QFile(set.read("main", "mainDir").toString() + "/Fonts/" + _font->fonts[currentFontIndex].alphaTexture()).exists())
        ui->lblAlphaTexture->setStyleSheet("color:red");

    if (ui->sbxMaxHeigthOfChars->text().isEmpty() || (_font->fonts[currentFontIndex].maxHeightOfChars() == 0))
        ui->lblMaxHeigthOfChars->setStyleSheet("color:red");

    if (ui->sbxDistanceBetweenChars->text().isEmpty())
        ui->lblDistanceBetweenChars->setStyleSheet("color:red");

    if (QFile(set.read("main", "mainDir").toString() + "/Fonts/" + _font->fonts[currentFontIndex].alphaTexture()).exists())
    {
        QImage alphaTexture(set.read("main", "mainDir").toString() + "/Fonts/" + _font->fonts[currentFontIndex].alphaTexture());

        if (alphaTexture.width() != 0 || alphaTexture.height() != 0)
        {
            if (_font->fonts[currentFontIndex].maxHeightOfChars() > QString::number(alphaTexture.height()).toInt())
                ui->lblMaxHeigthOfChars->setStyleSheet("color:red");

            if (_font->fonts[currentFontIndex].distanceBetweenChars() > QString::number(alphaTexture.width()).toInt())
                ui->lblDistanceBetweenChars->setStyleSheet("color:red");
        }
    }
}

void wdgGeneral::on_ledFontName_textChanged(const QString &arg1)
{
    _font->fonts[currentFontIndex].setName(arg1);
    checkPropValidity();
    emit checkCharValidity();
    emit setModified(true);
}

void wdgGeneral::on_ledColorTexture_textChanged(const QString &arg1)
{
    _font->fonts[currentFontIndex].setColorTexture(arg1);
    checkPropValidity();
    emit checkCharValidity();
    emit setModified(true);

    emit reloadPreview();
}

void wdgGeneral::on_ledAlphaTexture_textChanged(const QString &arg1)
{
    _font->fonts[currentFontIndex].setAlphaTexture(arg1);
    checkPropValidity();
    emit checkCharValidity();
    emit setModified(true);

    emit reloadPreview();
}

void wdgGeneral::on_btnColorTexture_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select color texture..."), set.read("main", "mainDir").toString() + "/Fonts", tr("Bitmap picture") + " (*.bmp)");

    if (filename.isEmpty()) return;

    _font->fonts[currentFontIndex].setColorTexture(filename.remove(0, QString(set.read("main", "mainDir").toString() + "/Fonts").size() + 1));

    if (ui->ledColorTexture->text() != _font->fonts[currentFontIndex].colorTexture())
    {
        qDebug() << QString("New color texture: '%1'").arg(filename);
        emit setModified(true);
    }

    ui->ledColorTexture->setText(_font->fonts[currentFontIndex].colorTexture());
}

void wdgGeneral::on_btnAlphaTexture_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select alpha texture..."), set.read("main", "mainDir").toString() + "/Fonts", tr("Bitmap picture") + " (*.bmp)");
    if (filename.isEmpty()) return;

    _font->fonts[currentFontIndex].setAlphaTexture(filename.remove(0, QString(set.read("main", "mainDir").toString() + "/Fonts").size() + 1));

    if (ui->ledAlphaTexture->text() != _font->fonts[currentFontIndex].alphaTexture())
    {
        qDebug() << QString("New alpha texture: '%1'").arg(filename);
        emit setModified(true);
    }

    ui->ledAlphaTexture->setText(_font->fonts[currentFontIndex].alphaTexture());
}

void wdgGeneral::on_sbxMaxHeigthOfChars_valueChanged(int arg1)
{
    _font->fonts[currentFontIndex].setMaxHeightOfChars(arg1);
    checkPropValidity();
    emit checkCharValidity();
    emit setModified(true);
}

void wdgGeneral::on_sbxDistanceBetweenChars_valueChanged(int arg1)
{
    _font->fonts[currentFontIndex].setDistanceBetweenChars(arg1);
    checkPropValidity();
    emit checkCharValidity();
    emit setModified(true);
}

void wdgGeneral::reloadUi()
{
    ui->ledFontName->setText(_font->fonts[currentFontIndex].name());
    ui->ledColorTexture->setText(_font->fonts[currentFontIndex].colorTexture());
    ui->ledAlphaTexture->setText(_font->fonts[currentFontIndex].alphaTexture());
    ui->sbxMaxHeigthOfChars->setValue(_font->fonts[currentFontIndex].maxHeightOfChars());
    ui->sbxDistanceBetweenChars->setValue(_font->fonts[currentFontIndex].distanceBetweenChars());

    checkPropValidity();
}

void wdgGeneral::on_cobxCurrentFont_currentIndexChanged(int index)
{
    if (setupFinished) /*TODO*/
    {
        emit fontIndexChanged(index);
    }
}

void wdgGeneral::changeFontIndex(int index)
{
    setupFinished = false;
    currentFontIndex = index;
    setupFinished = true;
}
