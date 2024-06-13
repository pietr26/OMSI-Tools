#ifndef WDGPREVIEW_H
#define WDGPREVIEW_H

#include <QWidget>
#include "OTBackend/OTOmsiFileHandler.h"

namespace Ui {
class wdgPreview;
}

class wdgPreview : public QWidget
{
    Q_OBJECT

public:
    explicit wdgPreview(QWidget *parent, OTFontModel &font);
    ~wdgPreview();

private:
    Ui::wdgPreview *ui;
};

#endif // WDGPREVIEW_H
