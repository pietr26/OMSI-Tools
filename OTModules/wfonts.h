#ifndef WFONTS_H
#define WFONTS_H

#include <QMainWindow>
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTOmsiFileHandler.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTGeneric/wselectencoding.h"
#include <QListWidgetItem>
#include <QItemSelectionModel>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include <QCloseEvent>
#include <QTimer>
#include <QShortcut>
#include <QGraphicsScene>

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

    void dragEnterEvent(QDragEnterEvent *e);

    void dropEvent(QDropEvent *e);

    void resizeEvent(QResizeEvent *event);

    void on_actionPreferences_triggered();

    void on_actionNewChar_triggered();

    void on_btnNewChar_clicked();

    void on_btnDeleteSelection_clicked();

    void on_actionDeleteSelection_triggered();

    void on_actionNewFont_triggered();

    void on_actionClose_triggered();

    void on_actionOpen_triggered();

    void on_actionSaveAs_triggered();

    void on_btnMoveUp_clicked();

    void on_btnMoveDown_clicked();

    void on_actionMoveCharUp_triggered();

    void on_actionMoveCharDown_triggered();

    void on_actionSave_triggered();

    void on_actionReload_triggered();

    void autosave();

    void on_actionFindChar_triggered();

    void on_ledFontName_textChanged(const QString &arg1);

    void on_ledColorTexture_textChanged(const QString &arg1);

    void on_ledAlphaTexture_textChanged(const QString &arg1);

    void on_actionGoToNextError_triggered();

    void on_actionLoadTemplate_triggered();

    void on_ledCharacter_textChanged(const QString &arg1);

    void on_ledComment_textChanged(const QString &arg1);

    void on_lvwChars_pressed(const QModelIndex &index);

    void on_actionCopyChars_triggered();

    void on_actionShowInExplorer_triggered();

    void on_btnAlphaTexture_clicked();

    void on_btnColorTexture_clicked();

    void on_btnFind_clicked();

    void on_btnNextResult_clicked();

    void on_btnCloseSearch_clicked();

    void on_ledSearch_textChanged(const QString &arg1);

    void on_ledSearch_returnPressed();

    void on_actionSendFeedback_triggered();

    void charSelectionChanged(const QModelIndex &newSel, const QModelIndex &oldSel);

    void on_sbxMaxHeigthOfChars_textChanged(const QString &arg1);

    void on_sbxDistanceBetweenChars_textChanged(const QString &arg1);

    void on_sbxLeftPixel_textChanged(const QString &arg1);

    void on_sbxRightPixel_textChanged(const QString &arg1);

    void on_sbxHighestPixelInFontRow_textChanged(const QString &arg1);

    void on_cobxPreviewOptions_currentIndexChanged(int index);

    void on_btnReloadTexPreview_clicked();

    void on_actionKeepPixelRowForNewCharacter_triggered();

    void on_actionDelete_triggered();

    void on_actionDuplicateCharacter_triggered();

    void on_actionOpenWithEncoding_triggered();

    void selectedEncoding(QStringConverter::Encoding selectedEncoding = QStringConverter::Latin1);

    void on_actionBackToHome_triggered();

signals:
    void backToHome();

private:
    Ui::wFonts *ui;
    wPreferences *WPREFERENCES;
    OTMessage msg;
    OTSettings set;
    OTMiscellaneous misc;
    OTFileOperations fop;
    OTOMSIFileHandler filehandler;
    QTimer *timer;

    bool unsaved = false;

    int maxRecentFileCount = 10;

    QStringListModel *strListChars;

    void setTitle(QString filen = "empty");

    QString save(OTFileMethods::fileMethods method, QString filen = "");

    void open(OTFileMethods::fileMethods method, QString filen = "", QStringConverter::Encoding encoding = QStringConverter::Latin1);

    void updateRecentFiles(QString newFile);

    void selectAllAndClear(bool onlyChar = false);

    void move(int selection, QString action);

    void enableFontArea(bool status);

    void enableView(bool status);

    void reloadCharList(bool addChar = false);

    void reloadCharUI();

    OTFontModel font;

    void loadRecentFiles();

    void saveRecentFiles(QString absoluteNewFilePath);

    void setUnsaved(bool state = true);

    void checkPropValidity();

    void checkCharValidity();

    QString currentSearch;

    bool charListUpdate = false;

    bool charUIUpdate = false;

    QGraphicsScene *texPreviewScene;

    void loadTexPreview();

    void resizeTexPreview();

    bool setupFinished = false;

    void reloadTotalCharLabel();
};

#endif // WFONTS_H
