#ifndef OTGLOBAL_H
#define OTGLOBAL_H

#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QDate>
#include <QDebug>
#include <QApplication>
#include <QProcess>
#include <QFileDialog>
#include <QtNetwork>
#include <QDesktopServices>
#include <QScreen>
#include <QClipboard>
#include <QString>

class OTFileMethods
{
public:
    enum methods
    {
        open = 1,
        reOpen = 2,
        silentOpen = 3,
        save = 4,
        saveAs = 5,
        backupSave = 6
    };
    Q_DECLARE_FLAGS(fileMethods, methods)
};

class OTBuildOptions
{
public:
    enum methods
    {
        Dev = 1,
        Alpha = 2,
        Beta = 3,
        EA = 4,
        Lite = 5,
        Prerelease = 6,
        Release = 7
    };
    Q_DECLARE_FLAGS(buildOptions, methods)

    static QString getBuildName(buildOptions flag)
    {
        switch (flag)
        {
        case Dev:           return "Dev";           break;
        case Alpha:         return "Alpha";         break;
        case Beta:          return "Beta";          break;
        case EA:            return "EA";            break;
        case Lite:          return "Lite";          break;
        case Prerelease:    return "Prerelease";    break;
        case Release:       return "Release";       break;
        default:            return "?";             break;
        }
    }
};

const QString OTName = "OMSI-Tools";
const QString OTVersion = "0.11.0-beta";
const OTBuildOptions::buildOptions OTBuild = OTBuildOptions::Beta;

// Unsaved code for closeEvents:
/*
    if (unsaved)
    {
        int msgResult = msg.unsavedContent(this);
        if (msgResult == -1)
        {
            event->ignore();
            return;
        }
        else if (msgResult == 1)
            save(OTFileMethods::save, font.path);
    }
*/


/// \brief Link collection
class OTLinks
{
public:
    inline static const QUrl wipThread = QUrl("https://reboot.omsi-webdisk.de/community/thread/4783-projekt-omsi-tools-das-allrounder-tool-heute-im-adventskalender/");
    inline static const QUrl supportThread = QUrl("https://reboot.omsi-webdisk.de/community/thread/5683-omsi-tools-support/");
    inline static const QUrl latestVersion = QUrl("http://omsi-tools.bplaced.net/omsi-tools/versionCheck/latestVersion.txt");
    inline static const QUrl changelog = QUrl("http://omsi-tools.bplaced.net/omsi-tools/changelog/index.html");
};

/// \brief Calculates disk usage
class OTDownloader: public QObject
{
    Q_OBJECT

public:
    /// \brief Returns the downloaded file
    QString doDownload(const QUrl &url)
    {
        lastSuccess = 0;
        return download(url);
    }

    /// \brief Saves the download file to a local file
    int doDownload(const QUrl &url, const QString filepath)
    {
        lastSuccess = 0;
        saveToFile(filepath, download(url));
        return lastHttpCode;
    }

    QNetworkReply *reply;
    int currentProgress;
    int maxProgress;
    int lastHttpCode;
    int currentContentLength;
    int lastSuccess;

private slots:
    void downloadProgress(qint64 current, qint64 max)
    {
        currentProgress = current;
        maxProgress = max;
    }

private:
    QNetworkAccessManager manager;

    /// \brief Main part of downloading a file
    QByteArray download(const QUrl &url)
    {
        qDebug().noquote().nospace() << "Download '" << url.url() << "'";
        QNetworkRequest request(url);
        QEventLoop loop;

        connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = manager.get(request);

        connect(reply, &QNetworkReply::downloadProgress, this, &OTDownloader::downloadProgress);
        loop.exec();
        currentContentLength = reply->header(QNetworkRequest::ContentLengthHeader).toInt();

        int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastHttpCode = httpCode;

        qDebug() << "HTTP Status code:" << httpCode;
        if (httpCode >= 300 || httpCode == 0)
        {
            lastSuccess = -1;
            return "";
        }
        else
            lastSuccess = 1;

        return reply->readAll();
    }

    /// \brief Saves the download file to a local file
    void saveToFile(const QString filepath, const QByteArray content)
    {
        QFile file(filepath);
        if (!file.open(QFile::WriteOnly))
            qWarning().noquote() << "Error while downloading and save a file from the internet!";
        else
            file.write(content);
    }
};

/// \brief Class for miscellaneous
class OTMiscellaneous
{
public:
    /// \brief Restarts the application
    void restart()
    {
        qInfo() << "Restart application...";
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }

    /// \brief Returns curreent time
    QString getTime(QString format = "hh:mm:ss")
    {
        return QTime::currentTime().toString(format);
    }

    /// \brief Returns current date
    QString getDate(QString format = "dd.MM.yyyy")
    {
        return QDate::currentDate().toString(format);
    }

    /// \brief Returns an universal file header
    QString writeFileHeader()
    {
        return "File created with " + OTName + " (Ver. " + OTVersion + ") on " + getDate() + ", " + getTime() + "\n\n";
    }

    void checkBackupFolderExistance()
    {
        if (!QDir("backup").exists())
            QDir().mkdir("backup");
    }

    /// \brief Checks for an update - index 0: "false" = error, "noUpdates" = no updates available, else: new version - index 1: latestVersion
    QStringList checkForUpdate()
    {
        QStringList list;

        qDebug() << "Check for update...";
        OTDownloader dl;
        const QString latestVersion = dl.doDownload(OTLinks::latestVersion);
        qDebug() << "Server response:" << latestVersion;

        if (latestVersion == "")
            list << "false" << latestVersion;
        else if (latestVersion == OTVersion)
            list << "noUpdates" << latestVersion;
        else
            list << latestVersion << latestVersion;

        return list;
    }

    static void sendFeedback()
    {
        QDesktopServices::openUrl(OTLinks::supportThread);
    }

    QSize sizeWindow(double width, double height)
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        QSize windowSize = screen->availableGeometry().size();
        windowSize.setWidth(windowSize.width() * width);
        windowSize.setHeight(windowSize.height() * height);

        return windowSize;
    }

    QPoint centerPosition(QWidget* parent)
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        return screen->geometry().center() - parent->rect().center();
    }

    /// \brief Copies a text
    void copy(QString copytext)
    {
        qDebug() << "Copy something...";

        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(copytext);
    }

    void createShortcut(QString filepath, QString shortcutLocation, QWidget *parent)
    {
        if (QFile(filepath).link(shortcutLocation))
        {
            QMessageBox::information(parent, QObject::tr("Success", "Note #1"), QObject::tr("Successfully created shortcut!"));
            qInfo().noquote() << QString("Created shortcut in '%1'!").arg(shortcutLocation);
        }
        else
        {
            QMessageBox::warning(parent, QString(QObject::tr("Error", "Note #1")), QObject::tr("Could not create shortcut in %1.").arg(QDir().homePath() + "/Desktop"));
            qCritical().noquote() << QString("Could not create a shortcut in '%1'!").arg(shortcutLocation);
        }

//        HRESULT hres;
//        IShellLink *psl;

//        hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
//        if (SUCCEEDED(hres))
//        {
//            IPersistFile* ppf;

//            psl->SetPath((const wchar_t*) filepath.utf16());
//            if (!args.isEmpty())
//                psl->SetArguments((const wchar_t*) QString(args).utf16());

//            hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

//            if (SUCCEEDED(hres))
//            {
//                WCHAR wsz[MAX_PATH];

//                MultiByteToWideChar(CP_ACP, 0, shortcutLocation.toStdString().c_str(), -1, wsz, MAX_PATH);

//                hres = ppf->Save(wsz, TRUE);
//                ppf->Release();
//            }
//            psl->Release();
//        }

        // ADD TO HEADER:
//#include "windows.h"
//#include "winnls.h"
//#include "shobjidl.h"
//#include "objbase.h"
//#include "objidl.h"
//#include "shlguid.h"
    }
};



/// \brief Class for settings which all modules needs
class OTSettings
{
public:
    /// \brief Writes a setting
    void write(QString module, QString name, QVariant value)
    {
        QSettings settings(OTName, module);
        settings.setValue(name, value);
    }

    /// \brief Reads a setting
    QVariant read(QString module, QString name, bool onlyValue = false)
    {
        QSettings settings(OTName, module);
        QVariant value = settings.value(name);
        //qDebug().noquote().nospace() << "Read settings from " << module << ": "<< name << ", value: " << value;

        if (name == "theme" && (!onlyValue))
        {
            if (value.toInt() == 1)
                return getStyleSheet("Combinear");
            else if (value.toInt() == 2)
                return getStyleSheet("Darkeum");
            else
                return "";
        }
        else
            return value;
    }

    /// \brief Select and control OMSI main dir path. Returns the OMSI path.
    QString getOmsiPath(QWidget *parent, QString path = "")
    {
        if (path == "")
            path = "C:/Program Files (x86)/Steam/steamapps/common/OMSI 2";

        QString mainDir = QFileDialog::getExistingDirectory(parent, QObject::tr("Select the OMSI main directory..."), path);

        if (mainDir != "" && !QFileInfo(QFile(mainDir + "/Omsi.exe")).exists())
        {
            qWarning().noquote() << "'" + mainDir + "' isn't an OMSI path!";
            QMessageBox::StandardButton reply = QMessageBox::warning(parent, QObject::tr("Could not found \"Omsi.exe\"", "Note #1"), QObject::tr("\"Omsi.exe\" could not found in the selected directory. Is it the correct path?. Otherwise, problems may appear in some modules. Should a new path be selected?"), QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
                return getOmsiPath(parent);
        }

        return mainDir;
    }

    /// \brief Returns the whole stylesheet
    QString getStyleSheet(QString name)
    {
        qDebug() << "Get stylesheet...";
        if (name != "")
        {
            QFile themePath(":/rec/data/themes/" + name + ".qss");
            if (!themePath.open(QFile::ReadOnly | QFile::Text))
                write("main", "theme", "");
            else
            {
                QTextStream in(&themePath);
                QString content = in.readAll();
                themePath.close();
                qDebug().noquote() << "Load stylesheet '" + QFileInfo(themePath).absoluteFilePath() + "'";
                return content;
            }

            qDebug().noquote() << "Could not find stlyesheet '" + QFileInfo(themePath).absoluteFilePath() + "'!";
        }
        return "";
    }

    /// \brief Gets all settings keys and its values
    QString getAllSettings()
    {
        QSettings set("HKEY_CURRENT_USER\\SOFTWARE\\" + OTName, QSettings::NativeFormat);

        QString returnString = "\n";

        foreach (QString current, set.allKeys())
            returnString += current + "\n " + set.value(current).toString() + "\n";

        return returnString;
    }

    /// \brief Installs a translator
    void loadLanguage(QApplication &parent)
    {
        QTranslator *translator = new QTranslator(&parent);
        QTranslator *baseTranslator = new QTranslator(&parent);

        QString languageFile;
        QString baseLanguageFile;
        switch (read("main", "language").toInt())
        {
        //case 0: english
        case 1: languageFile = "tr_de"; baseLanguageFile = "qtbase_de"; break;
        case 2: languageFile = "tr_fr"; baseLanguageFile = "qtbase_fr"; break;
        case 3: languageFile = "tr_it"; baseLanguageFile = "qtbase_it"; break;
        case 4: languageFile = "tr_cz"; baseLanguageFile = "qtbase_cz"; break;
        }

        if (languageFile != "")
        {
            bool trLoad = translator->load(languageFile, ":/rec/data/translations/");
            bool trInstall = parent.installTranslator(translator);

            bool baseTrLoad = baseTranslator->load(baseLanguageFile, ":/rec/data/translations/");
            bool baseTrInstall = parent.installTranslator(baseTranslator);

            if (trLoad && trInstall && baseTrLoad && baseTrInstall)
                qInfo() << "Loaded translations";
            else
            {
                qWarning() << "Error while loading translations!";

                qDebug().noquote() << "trLoad:" << trLoad;
                qDebug().noquote() << "trInstall:" << trInstall;
                qDebug().noquote() << "baseTrLoad:" << baseTrLoad;
                qDebug().noquote() << "baseTrInstall:" << baseTrInstall;
            }
        }
    }
};

/// \brief Message class
class OTMessage
{
public:
    /// \brief Asks if the user wants to set the main dir
    bool setMainDirYesNo(QWidget *parent)
    {
        qDebug() << "Message: Action needs main directory. Set now (Yes/No)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("OMSI main directory not found", "Note #1"), QObject::tr("To continue the application needs the OMSI main directory. Should it be done now?"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            return false;

        return true;
    }

    /// \brief "Module still work in Progress"
    void WIP(QWidget *parent)
    {
        QMessageBox::warning(parent, QObject::tr("Work in progress", "Note #1"), QObject::tr("Attention: This module is currently work in progress. Only use it for test, never for real modding! Files edited with this module can possibly be destroyed!"));
    }

    /// \brief universal unsaved-Message (with Save, Discard and Cancel). Returns 'save = 1', 'discard = 0' or 'cancel = -1'
    int unsavedContent(QWidget *parent)
    {
        qDebug() << "Message: Save unsaved content (Save/Discard/Cancel)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Unsaved content", "Note #1"), QObject::tr("There is unsaved content."), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (reply == QMessageBox::Save)
        {
            qDebug() << "User pressed Save";
            return 1;
        }
        else if (reply == QMessageBox::Discard)
        {
            qDebug() << "User pressed Discard";
            return 0;
        }
        else
        {
            qDebug() << "User pressed Cancel";
            return -1;
        }
    }

    /// \brief universal unsaved-Message (with Yes and No). Returns 'Yes = 1' or 'No = 0'
    bool unsavedContentYesNo(QWidget *parent)
    {
        qDebug() << "Message: Save unsaved content (Yes/No)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Unsaved content", "Note #1"), QObject::tr("There is unsaved content. Do you want to save?"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            qDebug() << "User pressed Yes";
            return true;
        }
        else
        {
            qDebug() << "User pressed No";
            return false;
        }
    }

    /// \brief If the user wants to open a file via a button and the file doesn't exist, this message es helpful
    void errorOpeningFile(QWidget *parent)
    {
        QMessageBox::information(parent, QObject::tr("Error while openig file", "Note #1"), QObject::tr("The selected file doesn't exists or is read-protected."));
    }

    /// \brief Could not open a file from OMSI main dir
    void errorWhileOpeningOmsi(QWidget *parent, QString filename)
    {
        QMessageBox::critical(parent, QObject::tr("Error while opening file", "Note #1"), QString(QObject::tr("There was an error while opening\n%1\nIf OMSI is running, please close it and retry it. Furthermore, check if the file still exists.")).arg(filename));
    }

    /// \brief Font module: No chars in font
    void noCharsInFont(QWidget *parent)
    {
        QMessageBox::warning(parent, QObject::tr("No chars in font", "Note #1"), QObject::tr("There are no chars in the font."));
    }

    /// \brief Comfirm a deletion of anything
    bool confirmDeletion(QWidget *parent)
    {
        if (set.read("main", "confirmDeletion") == "false")
            return true;

        qDebug() << "Message: Save unsaved content (Yes/No)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Confirm deletion", "Note #1"), QObject::tr("Should the selection be deleted?"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            qDebug() << "User pressed Yes";
            return true;
        }
        else
        {
            qDebug() << "User pressed No";
            return false;
        }
    }

    /// \brief There was en error while saving a file
    void errorWhileSaving(QString filename, QWidget *parent = 0)
    {
        QMessageBox::warning(parent, QObject::tr("Save error", "Note #1"), QString(QObject::tr("There was an save error '%1'. If OMSI is running, please close it and retry it.")).arg(filename));
    }

    /// \brief A module is due to narrow time deactivated
    void moduleDeactivated(QWidget *parent = 0)
    {
        QMessageBox::information(parent, QObject::tr("Module still deactivated", "Note #1"), QObject::tr("Unfortunately, this module is still deactivated due to a time shortage in the developments. Please check for new updates in the next few days."));
    }

private:
    OTSettings set;
};

/// \brief Calculates disk usage
class OTDiskUsage
{
public:
    OTDiskUsage()
    {
        units << QObject::tr("Bytes") << QObject::tr("KB", "Short version of 'Kilobyte'") <<
                 QObject::tr("MB", "Short version of 'Megabyte'") << QObject::tr("GB", "Short version of 'Gigabyte'") <<
                 QObject::tr("TB", "Short version of 'Terrabyte'") << QObject::tr("PB", "Short version of 'Petabyte'");
    }

    /// \brief Returns disk usage of 'dirPath'
    QString formatSize(QString dirPath)
    {
        qint64 size = dirSize(dirPath);

        int i;
        double outputSize = size;
        for(i = 0; i < units.size() - 1; i++)
        {
            if (outputSize < 1024)
                break;

            outputSize = outputSize / 1024;
        }
        QString result = QString("%0 %1").arg(outputSize, 0, 'f', 2).arg(units[i]);
        return result;
    }

    QString fileSize(QString filename)
    {
        qint64 size = QFileInfo(QFile(filename)).size();

        int i;
        double outputSize = size;
        for(i = 0; i < units.size() - 1; i++)
        {
            if (outputSize < 1024)
                break;

            outputSize = outputSize / 1024;
        }
        QString result = QString("%0 %1").arg(outputSize, 0, 'f', 2).arg(units[i]);
        return result;
    }

    QString calculateSize(qint64 size)
    {
        int i;
        double outputSize = size;
        for(i = 0; i < units.size() - 1; i++)
        {
            if (outputSize < 1024)
                break;

            outputSize = outputSize / 1024;
        }
        QString result = QString("%0 %1").arg(outputSize, 0, 'f', 2).arg(units[i]);
        return result;
    }

private:
    qint64 dirSize(QString dirPath)
    {
        qint64 size = 0;
        QDir dir(dirPath);

        // calculate total size of current directories' files
        QDir::Filters fileFilters = QDir::Files | QDir::System | QDir::Hidden;
        for (QString filePath : dir.entryList(fileFilters))
        {
            QFileInfo fi(dir, filePath);
            size += fi.size();
        }

        // add size of child directories recursively
        QDir::Filters dirFilters = QDir::Dirs | QDir::NoDotAndDotDot | QDir::System | QDir::Hidden;

        for (QString childDirPath : dir.entryList(dirFilters))
            size += dirSize(dirPath + QDir::separator() + childDirPath);

        return size;
    }

    QStringList units;
};

#endif // OTGLOBAL_H
