#include "wdgpreview.h"
#include "ui_wdgpreview.h"

wdgPreview::wdgPreview(QWidget *parent, OCFont::FontCollection *font)
    : QWidget(parent)
    , ui(new Ui::wdgPreview),
    _font(font)
{
    ui->setupUi(this);

    reloadUi();
    ui->cobxPreviewOptions->setCurrentIndex(set.read(objectName(), "texPreview").toInt());

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
    set.write(objectName(), "texPreview", index);
    reloadUi();
}

void wdgPreview::on_btnReloadTexPreview_clicked()
{
    reloadUi();
}

void wdgPreview::reloadUi(bool reset, bool charChange)
{
    Q_UNUSED(reset); Q_UNUSED(charChange);

    if (_font->selection.contains(OCFont::FontCollection::FontSelection))
    {
        QString tex = set.read("main", "mainDir").toString() + "/Fonts/" + (set.read(objectName(), "texPreview").toInt() == 0 ? _font->fonts[_font->selection[OCFont::FontCollection::FontSelection]]->colorTexture : _font->fonts.at(_font->selection[OCFont::FontCollection::FontSelection])->alphaTexture);

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

    ui->btnReloadTexPreview->setEnabled(_font->selection.contains(OCFont::FontCollection::FontSelection));
    ui->cobxPreviewOptions->setEnabled(_font->selection.contains(OCFont::FontCollection::FontSelection));
}

void wdgPreview::resizeTexPreview()
{
    grv->fitInView(texPreviewScene->sceneRect(), Qt::KeepAspectRatio);
}
