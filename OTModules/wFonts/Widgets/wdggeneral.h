#ifndef WDGGENERAL_H
#define WDGGENERAL_H

#include <QWidget>
#include "OTBackend/OTOmsiFileHandler.h"

namespace Ui {
class wdgGeneral;
}

class wdgGeneral : public QWidget
{
    Q_OBJECT

public:
    explicit wdgGeneral(QWidget *parent, OTFontModel &font);
    ~wdgGeneral();

private:
    Ui::wdgGeneral *ui;
};

#endif // WDGGENERAL_H
