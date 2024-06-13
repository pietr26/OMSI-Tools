#ifndef WDGCHARS_H
#define WDGCHARS_H

#include <QWidget>
#include "OTBackend/OTOmsiFileHandler.h"

namespace Ui {
class wdgChars;
}

class wdgChars : public QWidget
{
    Q_OBJECT

public:
    explicit wdgChars(QWidget *parent, OTFontModel &font);
    ~wdgChars();

private:
    Ui::wdgChars *ui;
};

#endif // WDGCHARS_H
