#ifndef DLGFILEPREVIEW_H
#define DLGFILEPREVIEW_H

#include <QDialog>
#include "OTBackend/OTContentValidator/OTContentValidator.h"
#include <QTreeWidgetItem>

namespace Ui {
class dlgFilePreview;
}

class dlgFilePreview : public QDialog
{
    Q_OBJECT

public:
    explicit dlgFilePreview(QWidget *parent, const QString &filePath, const OTContentValidatorResult &result);
    ~dlgFilePreview();

    static void openExternally(QString filePath);
    static bool isBinaryFile(const QString &filePath);

protected:
    void openFile(const QString &filePath, const OTContentValidatorResult &result);
    void loadFile(const QString &filePath);

private slots:
    void clear();
    void on_btnOpenFile_clicked();

    void on_twgFiles_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_twgPreview_itemClicked(QTreeWidgetItem *item, int column);

    void on_btnOpenDirectory_clicked();

private:
    Ui::dlgFilePreview *ui;

    QHash<QString, OTContentValidatorResult> _results;
    OTContentValidatorResult _currentResult;
    inline static const QStringList _binaryFileTypes = {"o3d", "bmp", "png", "tga", "dds", "jpg", "wav", "terrain", "rdy", "water", "dll", "exe", "surf"};
};

#endif // DLGFILEPREVIEW_H
