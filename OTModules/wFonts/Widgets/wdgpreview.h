#ifndef WDGPREVIEW_H
#define WDGPREVIEW_H

#include <QWidget>
#include "OTBackend/OTOmsiFileHandler.h"
#include <QGraphicsScene>

namespace Ui {
class wdgPreview;
}

class wdgPreview : public QWidget
{
    Q_OBJECT

public:
    explicit wdgPreview(QWidget *parent, OTFontModel *font);
    ~wdgPreview();

private slots:
    void on_cobxPreviewOptions_currentIndexChanged(int index);

    void on_btnReloadTexPreview_clicked();

public slots:
    void reloadUi();

private:
    Ui::wdgPreview *ui;

    OTSettings set;

    OTFontModel *_font;

    void resizeTexPreview();

    QGraphicsScene *texPreviewScene = new QGraphicsScene();
};

#endif // WDGPREVIEW_H
