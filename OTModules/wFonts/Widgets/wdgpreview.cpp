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
    ui->hslOpacity->setValue(set.read(objectName(), "texPreviewOpacity").toInt());

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

    int hScroll = grv->horizontalScrollBar()->value();
    int vScroll = grv->verticalScrollBar()->value();

    bool hasFontSelection = _font->selection.contains(OCFont::FontCollection::FontSelection);
    ui->btnReloadTexPreview->setEnabled(hasFontSelection);
    ui->cobxPreviewOptions->setEnabled(hasFontSelection);

    texPreviewScene->clear();

    if (hasFontSelection)
    {
        QString tex = set.read("main", "mainDir").toString() + "/Fonts/" +
                      (set.read(objectName(), "texPreview").toInt() == 0 ?
                           _font->fonts[_font->selection[OCFont::FontCollection::FontSelection]]->colorTexture :
                           _font->fonts.at(_font->selection[OCFont::FontCollection::FontSelection])->alphaTexture);

        if (QFile(tex).exists())
        {
            QPixmap baseTexture = QPixmap(tex);

            // Overlay
            if (_font->selection.contains(OCFont::FontCollection::CharacterSelection))
            {
                overlay = QPixmap(baseTexture.size());
                overlay.fill(Qt::transparent);

                QPainter painter(&overlay);
                QPen pen(QColor(255, 0, 0, set.read(objectName(), "texPreviewOpacity").isValid() ? set.read(objectName(), "texPreviewOpacity").toInt() : 255));
                pen.setWidth(1);
                painter.setPen(pen);

                OCFont::SingleFont *font = _font->fonts.at(_font->selection.value(OCFont::FontCollection::FontSelection));
                OCFont::Character *character = font->characters.at(_font->selection.value(OCFont::FontCollection::CharacterSelection));

                painter.drawRect(character->leftPixel, character->highestPixelInFontRow, character->rightPixel - character->leftPixel, font->maxHeightOfChars);
                painter.end();
            }
            else if (_font->selection.contains(OCFont::FontCollection::FontSelection))
            {
                overlay = QPixmap(baseTexture.size());
                overlay.fill(Qt::transparent);

                QPainter painter(&overlay);
                QPen pen(QColor(255, 0, 0, set.read(objectName(), "texPreviewOpacity").isValid() ? set.read(objectName(), "texPreviewOpacity").toInt() : 255));
                pen.setWidth(1);
                painter.setPen(pen);

                OCFont::SingleFont *font = _font->fonts.at(_font->selection.value(OCFont::FontCollection::FontSelection));

                foreach (OCFont::Character *current, font->characters)
                    painter.drawRect(current->leftPixel, current->highestPixelInFontRow, current->rightPixel - current->leftPixel, font->maxHeightOfChars);

                painter.end();
            }

            QPainter painter(&baseTexture);
            painter.drawPixmap(0, 0, overlay);
            painter.end();

            texPreviewScene->addPixmap(baseTexture);
        }
    }

    grv->horizontalScrollBar()->setValue(hScroll);
    grv->verticalScrollBar()->setValue(vScroll);
}

void wdgPreview::on_hslOpacity_valueChanged(int value)
{
    set.write(objectName(), "texPreviewOpacity", value);
    reloadPreview(new OCFont::FontCollection(), false);
}
