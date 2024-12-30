#include "wdgpreview.h"
#include "ui_wdgpreview.h"

wdgPreview::wdgPreview(QWidget *parent, OCFont::FontCollection *font)
    : QWidget(parent)
    , ui(new Ui::wdgPreview),
    _font(font)
{
    ui->setupUi(this);

    reloadPreview(new OCFont::FontCollection());
    ui->cobxPreviewOptions->setCurrentIndex(set.read(objectName(), "texPreview").toInt());

    // First setup - if not, the application will crash in wFonts::resizeEvent()
    texPreviewScene = new QGraphicsScene(this);
    layout()->addWidget(grv);
    grv->setScene(texPreviewScene);
    grv->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
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
    reloadPreview(new OCFont::FontCollection(), false);
}

void wdgPreview::on_btnReloadTexPreview_clicked()
{
    reloadPreview(new OCFont::FontCollection(), false);
}

void wdgPreview::reloadPreview(OCFont::FontCollection *font, bool update)
{
    if (update) _font = font;

    ui->btnReloadTexPreview->setEnabled(_font->selection.contains(OCFont::FontCollection::FontSelection));
    ui->cobxPreviewOptions->setEnabled(_font->selection.contains(OCFont::FontCollection::FontSelection));

    if (_font->selection.contains(OCFont::FontCollection::FontSelection))
    {
        QString tex = set.read("main", "mainDir").toString() + "/Fonts/" + (set.read(objectName(), "texPreview").toInt() == 0 ? _font->fonts[_font->selection[OCFont::FontCollection::FontSelection]]->colorTexture : _font->fonts.at(_font->selection[OCFont::FontCollection::FontSelection])->alphaTexture);

        // QGraphicsScene::clear() is not enough - it doesn't reset the draw aera size
        texPreviewScene = new QGraphicsScene(this);
        grv->setScene(texPreviewScene);

        if (QFile(tex).exists())
        {
            QPixmap baseTexture = QPixmap(tex);

            // Overlay

            if (update && _font->selection.contains(OCFont::FontCollection::CharacterSelection))
            {
                overlay = QPixmap(baseTexture.size());
                overlay.fill(Qt::transparent);

                QPainter painter(&overlay);
                QPen pen(QColor(255, 0, 0, 100));
                pen.setWidth(1);
                painter.setPen(pen);

                OCFont::SingleFont *font = _font->fonts.at(_font->selection.value(OCFont::FontCollection::FontSelection));
                OCFont::Character *character = font->characters.at(_font->selection.value(OCFont::FontCollection::CharacterSelection));

                painter.drawRect(character->leftPixel, character->highestPixelInFontRow, character->rightPixel - character->leftPixel, font->maxHeightOfChars);
                painter.end();
            }
            else if (update && _font->selection.contains(OCFont::FontCollection::FontSelection))
            {
                overlay = QPixmap(baseTexture.size());
                overlay.fill(Qt::transparent);

                QPainter painter(&overlay);
                QPen pen(QColor(255, 0, 0, 100));
                pen.setWidth(1);
                painter.setPen(pen);

                OCFont::SingleFont *font = _font->fonts.at(_font->selection.value(OCFont::FontCollection::FontSelection));

                foreach (OCFont::Character *current, font->characters)
                    painter.drawRect(current->leftPixel, current->highestPixelInFontRow, current->rightPixel - current->leftPixel, font->maxHeightOfChars);

                painter.end();
            }

            // ---

            QPainter painter(&baseTexture);
            painter.drawPixmap(0, 0, overlay);
            painter.end();

            texPreviewScene->addPixmap(baseTexture);
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
    //grv->fitInView(texPreviewScene->sceneRect(), Qt::KeepAspectRatio);
}
