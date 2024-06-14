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

#include "OTModules/OTGeneric/wselectencoding.h"

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

    void on_actionNewFont_triggered();

    void on_actionOpen_triggered();

    void on_actionOpenWithEncoding_triggered();

    void on_actionReload_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionShowInExplorer_triggered();

signals:
    void backToHome();

    void reloadUi();

private:
    Ui::wFonts *ui;
    wPreferences *WPREFERENCES;
    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;
    OTOMSIFileHandler filehandler;

    wdgGeneral *WDGGENERAL;
    wdgChars *WDGCHARS;
    wdgPreview *WDGPREVIEW;

    wSelectEncoding *WSELECTENCODING;

    OTFontModel *_font;

    int maxRecentFileCount = 10;

    void createDockWidgets();
    QString save(OTFileMethods::fileMethods method, QString filen = "");
    void setTitle(QString filen = "");
    void saveRecentFiles(QString absoluteNewFilePath);
    void loadRecentFiles();
    void open(OTFileMethods::fileMethods method, QString filen = "", QStringConverter::Encoding encoding = QStringConverter::Latin1);
    void selectedEncoding(QStringConverter::Encoding selectedEncoding);
};

#endif // WFONTS_H
