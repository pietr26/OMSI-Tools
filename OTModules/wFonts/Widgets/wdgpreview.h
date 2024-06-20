#ifndef WDGPREVIEW_H
#define WDGPREVIEW_H

#include <QWidget>
#include "OTBackend/OCC.h"
#include <QGraphicsScene>

namespace Ui {
class wdgPreview;
}

class wdgPreview : public QWidget
{
    Q_OBJECT

public:
    explicit wdgPreview(QWidget *parent, OCFont *font);
    ~wdgPreview();

private slots:
    void on_cobxPreviewOptions_currentIndexChanged(int index);

    void on_btnReloadTexPreview_clicked();

public slots:
    void reloadUi();

    void resizeTexPreview();

    void resizeEvent(QResizeEvent *event);

private:
    Ui::wdgPreview *ui;

    OTSettings set;

    OCFont *_font;

    QGraphicsScene *texPreviewScene = new QGraphicsScene();
};

#endif // WDGPREVIEW_H
