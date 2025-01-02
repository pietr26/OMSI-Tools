#ifndef WFONTS_H
#define WFONTS_H

#include <QMainWindow>
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTBackend/OCC/OCFont.h"
#include <QDockWidget>
#include "Widgets/wdgeditor.h"
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
    void closeEvent (QCloseEvent *event);

    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

    void on_actionNewFile_triggered();

    void on_actionOpen_triggered();

    void on_actionOpenWithEncoding_triggered();

    void on_actionReload_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionShowInExplorer_triggered();

    void on_actionSendFeedback_triggered();

    void on_actionPreferences_triggered();

signals:
    void backToHome();

    void reloadUi(bool reset = false, bool charChange = false);
    void reloadPreview();

private:
    Ui::wFonts *ui;
    wPreferences *WPREFERENCES;
    OTSettings set;
    OTMessage msg;
    OTMiscellaneous misc;

    wdgEditor *WDGEDITOR;
    wdgPreview *WDGPREVIEW;

    wSelectEncoding *WSELECTENCODING;

    OCFont::FontCollection *_font;

    int maxRecentFileCount = 10;

    void createDockWidgets();
    void save(OTFileMethods::fileMethods method, QString filen = "");
    void setTitle(QString filen = "");
    void saveRecentFiles(QString absoluteNewFilePath);
    void loadRecentFiles();
    void open(OTFileMethods::fileMethods method, QString filen = "", QStringConverter::Encoding encoding = QStringConverter::Latin1);
    void selectedEncoding(QStringConverter::Encoding selectedEncoding);

    void setVisiblilty();
};

#endif // WFONTS_H
