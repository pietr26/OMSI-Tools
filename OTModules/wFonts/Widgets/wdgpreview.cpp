#include "wdgpreview.h"
#include "ui_wdgpreview.h"

wdgPreview::wdgPreview(QWidget *parent, OTFontModel *font)
    : QWidget(parent)
    , ui(new Ui::wdgPreview),
    _font(font)
{
    ui->setupUi(this);

    reloadUi();

    // First setup - if not, the application will crash in wFonts::resizeEvent()
    texPreviewScene = new QGraphicsScene(this);
    ui->grvTexPreview->setScene(texPreviewScene);
}

wdgPreview::~wdgPreview()
{
    delete ui;
}

void wdgPreview::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeTexPreview();
}

void wdgPreview::on_cobxPreviewOptions_currentIndexChanged(int index)
{
    if (/*setupFinished*/ true) /*TODO*/set.write(objectName(), "texPreview", index);
    reloadUi();
}


void wdgPreview::on_btnReloadTexPreview_clicked()
{
    reloadUi();
}

void wdgPreview::reloadUi()
{
    QString tex;

    if (set.read(objectName(), "texPreview").toInt() == 0)
        tex = set.read("main", "mainDir").toString() + "/Fonts/" + _font->colorTexture;
    else
        tex = set.read("main", "mainDir").toString() + "/Fonts/" + _font->alphaTexture;

    ui->cobxPreviewOptions->setCurrentIndex(set.read(objectName(), "texPreview").toInt());

    if (QFile(tex).exists())
    {
        // 'QGraphicsScene::clear()' is not enough - it doesn't reset the draw aera size
        texPreviewScene = new QGraphicsScene(this);
        ui->grvTexPreview->setScene(texPreviewScene);

        texPreviewScene->addPixmap(tex);
        resizeTexPreview();
    }
}

void wdgPreview::resizeTexPreview()
{
    ui->grvTexPreview->fitInView(texPreviewScene->sceneRect(), Qt::KeepAspectRatio);
}
