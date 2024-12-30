#ifndef WDGPREVIEW_H
#define WDGPREVIEW_H

#include <QWidget>
#include "OTBackend/OCC/OCFont.h"
#include <QGraphicsScene>
#include "OTWidgets/graphicsview.h"
#include "OTBackend/OCC/OCFont.h"
#include "wdggraphicsview.h"

namespace Ui {
class wdgPreview;
}

class wdgPreview : public QWidget
{
    Q_OBJECT

public:
    explicit wdgPreview(QWidget *parent, OCFont::FontCollection *font);
    ~wdgPreview();

private slots:
    void on_cobxPreviewOptions_currentIndexChanged(int index);

    void on_btnReloadTexPreview_clicked();

    void on_hslOpacity_valueChanged(int value);

public slots:
    void reloadPreview();

    void resizeEvent(QResizeEvent *event);

private:
    Ui::wdgPreview *ui;

    OTSettings set;

    OCFont::FontCollection *_font;

    QPixmap overlay;

    QGraphicsScene *texPreviewScene = new QGraphicsScene();
    wdgGraphicsView *grv = new wdgGraphicsView();

    void drawCharacterOverlay();
};

#endif // WDGPREVIEW_H
