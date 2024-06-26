#ifndef WDEVTOOLS_H
#define WDEVTOOLS_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wDevTools;
}

class wDevTools : public QMainWindow
{
    Q_OBJECT

public:
    explicit wDevTools(QWidget *parent = nullptr);
    ~wDevTools();

private slots:

    void on_btnGetListOfFiles_clicked();

    void on_btnGetKeywords_clicked();

    void on_tbnOutputPathSelection_clicked();

    void on_btnLineCounter_clicked();

    void on_rbnOutputToGUI_clicked();

    void on_rbnOutputToFile_clicked();

    void on_btnSoundFileLister_clicked();

    void on_btnLineCounterStart_clicked();

    void on_actionBackToHome_triggered();

    void on_actionClose_triggered();

signals:
    void backToHome();

private:
    Ui::wDevTools *ui;
    OTSettings set;
    OTMiscellaneous misc;
    QString omsiFilesFilter = "Sceneryobjects (*.sco);;"
                     "Splines (*.sli);;"
                     "Vehicles (*.bus);;"
                     "AI vehicles (*.ovh);;"
                     "Fonts (*.oft);;"
                     "Configuration files (*.cfg);;"
                     "Tiles (*.map);;"
                     "Translations (*.dsc);;"
                     "Situations (*.osn);;"
                     "Weather (*.owt);;"
                     "All files (*.*)";
    int cutCount = set.read("main", "mainDir").toString().size() + 1;

    bool pushToOutput(QString result);

    void setOutputVisible(bool mode);

    QStringList countLinesFiles;

};

#endif // WDEVTOOLS_H
