#ifndef WFONTS_H
#define WFONTS_H

#include <QMainWindow>
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTOmsiFileHandler.h"
#include <QDockWidget>
#include "Widgets/wdggeneral.h"
#include "Widgets/wdgchars.h"
#include "Widgets/wdgpreview.h"

namespace Ui {
class wFonts;
}

class wFonts : public QMainWindow
{
    Q_OBJECT

public:
    explicit wFonts(QWidget *parent = nullptr);
    ~wFonts();

private slots:
    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

signals:
    void backToHome();

private:
    Ui::wFonts *ui;
    wPreferences *WPREFERENCES;
    OTSettings set;
    OTOMSIFileHandler filehandler;

    wdgGeneral *WDGGENERAL = new wdgGeneral(this, font);
    wdgChars *WDGCHARS = new wdgChars(this, font);
    wdgPreview *WDGPREVIEW = new wdgPreview(this, font);

    OTFontModel font;

    void createDockWidgets();
};

#endif // WFONTS_H
