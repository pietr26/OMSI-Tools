#include "wdgpreview.h"
#include "ui_wdgpreview.h"

wdgPreview::wdgPreview(QWidget *parent, OCFont *font)
    : QWidget(parent)
    , ui(new Ui::wdgPreview),
    _font(font)
{
    ui->setupUi(this);

    reloadUi();

    // First setup - if not, the application will crash in wFonts::resizeEvent()
    texPreviewScene = new QGraphicsScene(this);
    layout()->addWidget(grv);
    grv->setScene(texPreviewScene);
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
    if (/*setupFinished*/ true) /*TODO*/set.write(objectName(), "texPreview", index); // BUG
    reloadUi();
}


void wdgPreview::on_btnReloadTexPreview_clicked()
{
    reloadUi();
}

void wdgPreview::reloadUi()
{
    if (_font->selection.contains(OCFont::Selection::Font))
    {
        QString tex = set.read("main", "mainDir").toString() + "/Fonts/" + (set.read(objectName(), "texPreview").toInt() == 0 ? _font->fonts[_font->selection[OCFont::Selection::Font]].colorTexture() : _font->fonts[_font->selection[OCFont::Selection::Font]].alphaTexture());

        ui->cobxPreviewOptions->setCurrentIndex(set.read(objectName(), "texPreview").toInt());

        // QGraphicsScene::clear() is not enough - it doesn't reset the draw aera size
        texPreviewScene = new QGraphicsScene(this);
        grv->setScene(texPreviewScene);

        if (QFile(tex).exists())
        {
            texPreviewScene->addPixmap(tex);
            resizeTexPreview();
        }
    }
    else
    {
        texPreviewScene = new QGraphicsScene(this);
        grv->setScene(texPreviewScene);
    }
}

void wdgPreview::resizeTexPreview()
{
    grv->fitInView(texPreviewScene->sceneRect(), Qt::KeepAspectRatio);
}
