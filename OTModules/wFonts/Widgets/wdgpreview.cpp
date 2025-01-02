#include "wdgpreview.h"
#include "ui_wdgpreview.h"

wdgPreview::wdgPreview(QWidget *parent, OCFont::FontCollection *font)
    : QWidget(parent)
    , ui(new Ui::wdgPreview),
    _font(font)
{
    ui->setupUi(this);

    ui->cobxPreviewOptions->setCurrentIndex(set.read(objectName(), "texPreview").toInt());
    ui->hslOpacity->setValue(set.read(objectName(), "texPreviewOpacity").toInt());
    ui->cbxFollowSelection->setChecked(set.read(objectName(), "followSelection").toBool());

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
    reloadPreview();
}

void wdgPreview::on_btnReloadTexPreview_clicked()
{
    reloadPreview();
}

void wdgPreview::reloadPreview()
{
    int hScroll = grv->horizontalScrollBar()->value();
    int vScroll = grv->verticalScrollBar()->value();

    bool hasFontSelection = _font->selection.contains(OCFont::FontCollection::FontSelection);
    ui->btnReloadTexPreview->setEnabled(hasFontSelection);
    ui->cobxPreviewOptions->setEnabled(hasFontSelection);

    texPreviewScene->clear();
    xValues.clear();
    yValues.clear();

    if (_font->fonts.isEmpty()) return;

    if (hasFontSelection)
    {
        QString tex = set.read("main", "mainDir").toString() + "/Fonts/" +
                      (set.read(objectName(), "texPreview").toInt() == 0 ?
                           _font->fonts[_font->selection[OCFont::FontCollection::FontSelection]]->colorTexture :
                           _font->fonts.at(_font->selection[OCFont::FontCollection::FontSelection])->alphaTexture);

        if (QFileInfo(tex).isFile() && QFile(tex).exists())
        {
            QPixmap baseTexture = QPixmap(tex);

            QList<OCFont::Character*> characters;

            OCFont::SingleFont *font = _font->fonts.at(_font->selection.value(OCFont::FontCollection::FontSelection));

            if (_font->selection.contains(OCFont::FontCollection::CharacterSelection))
                characters << font->characters.at(_font->selection.value(OCFont::FontCollection::CharacterSelection));
            else if (_font->selection.contains(OCFont::FontCollection::FontSelection))
                characters << font->characters;

            paintOverlay(characters, font->maxHeightOfChars, baseTexture);

            texPreviewScene->addPixmap(baseTexture);

            if (!xValues.isEmpty() && set.read(objectName(), "followSelection").toBool())
            {
                std::sort(xValues.begin(), xValues.end());
                std::sort(yValues.begin(), yValues.end());

                QRect viewRect(xValues.first(), yValues.first(), std::abs(xValues.last() - xValues.first()), font->maxHeightOfChars);
                grv->fitInView(viewRect, Qt::KeepAspectRatio);
            }
            else
            {
                grv->horizontalScrollBar()->setValue(hScroll);
                grv->verticalScrollBar()->setValue(vScroll);
            }
        }
    }
}

void wdgPreview::paintOverlay(QList<OCFont::Character*> characters, int maxHeightOfChars, QPixmap &pixmap)
{
    QPainter painter(&pixmap);
    QPen pen(QColor(255, 0, 0, set.read(objectName(), "texPreviewOpacity").isValid() ? set.read(objectName(), "texPreviewOpacity").toInt() : 255));
    pen.setWidth(1);
    painter.setPen(pen);

    foreach (OCFont::Character *current, characters)
    {
        xValues << current->leftPixel << current->rightPixel;
        yValues << current->highestPixelInFontRow;
        painter.drawRect(current->leftPixel, current->highestPixelInFontRow, current->rightPixel - current->leftPixel, maxHeightOfChars);
    }

    painter.end();
}

void wdgPreview::on_hslOpacity_valueChanged(int value)
{
    set.write(objectName(), "texPreviewOpacity", value);
    reloadPreview();
}

void wdgPreview::on_cbxFollowSelection_stateChanged(int arg1)
{
    set.write(objectName(), "followSelection", bool(arg1));
    reloadPreview();
}
