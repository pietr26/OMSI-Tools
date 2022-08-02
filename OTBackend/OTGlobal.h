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
        default:            return "undefined";     break;
        }
    }
};

const QString OTName = "OMSI-Tools";
const QString OTVersion = "1.0.1-beta";
const OTBuildOptions::buildOptions OTBuild = OTBuildOptions::Dev;

const QList<QPair<QString, unsigned int>> OTFacts(
            {
                QPair<QString, unsigned int>("0,5s", 500),
                QPair<QString, unsigned int>("1s", 1000),
                QPair<QString, unsigned int>("3s", 3000),
                QPair<QString, unsigned int>("5s", 5000)
            }
            );

/// Unsaved code snippet for closeEvents:
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


/// [STATIC] Link collection
class OTLinks
{
public:
    inline static const QUrl showroom = QUrl("https://reboot.omsi-webdisk.de/community/thread/4783");
    inline static const QUrl support = QUrl("https://reboot.omsi-webdisk.de/community/thread/5683");
    inline static const QUrl latestVersion = QUrl("http://omsi-tools.bplaced.net/omsi-tools/versionCheck/latestVersion.txt");
    inline static const QUrl releaseNotes = QUrl("http://omsi-tools.bplaced.net/omsi-tools/releaseNotes/index.html");
    inline static const QUrl download = QUrl("http://omsi-tools.bplaced.net/omsi-tools/download/currentDownloadLink.php");
    inline static const QUrl survey = QUrl("https://forms.office.com/r/QbbQBQCa21");
    inline static const QUrl github = QUrl("https://github.com/pietr26/OMSI-Tools");
};

/// Calculates disk usage
class OTDownloader: public QObject
{
    Q_OBJECT
public slots:
    /// [OVERLOADED] Returns the downloaded file
    QString doDownload(const QUrl &url)
    {
        lastSuccess = 0;
        return download(url);
    }

    /// [OVERLOADED] Saves the download file to a local file
    int doDownload(const QUrl &url, const QString filepath)
    {
        lastSuccess = 0;
        saveToFile(filepath, download(url));
        return lastHttpCode;
    }

public:
    QNetworkReply *reply;
    int currentProgress;
    int maxProgress;
    int lastHttpCode;

    /// Check if the last connection was successfully. -1: No | 0: NULL | 1: Yes
    int lastSuccess;

private slots:
    void downloadProgress(qint64 current, qint64 max)
    {
        currentProgress = current;
        maxProgress = max;
    }

private:
    QNetworkAccessManager manager;

    /// Main part of downloading a file
    QByteArray download(const QUrl &url)
    {
        qDebug().noquote().nospace() << "Download '" << url.url() << "'";
        QNetworkRequest request(url);
        QEventLoop loop;

        connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = manager.get(request);

        connect(reply, &QNetworkReply::downloadProgress, this, &OTDownloader::downloadProgress);
        loop.exec();

        // Content length:
        // reply->header(QNetworkRequest::ContentLengthHeader).toInt();

        int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastHttpCode = httpCode;

        qDebug() << "Download finished.";
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

    /// Saves the download file to a local file
    void saveToFile(const QString filepath, const QByteArray content)
    {
        QFile file(filepath);
        if (!file.open(QFile::WriteOnly))
            qWarning().noquote() << "Error while downloading and save a file from the internet!";
        else
            file.write(content);
    }
};

/// Class for miscellaneous
class OTMiscellaneous
{
public:
    /// Restarts the application
    void restart()
    {
        qInfo() << "Restart application...";
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }

    /// Returns curreent time
    QString getTime(QString format = "hh:mm:ss")
    {
        return QTime::currentTime().toString(format);
    }

    /// Returns current date
    QString getDate(QString format = "dd.MM.yyyy")
    {
        return QDate::currentDate().toString(format);
    }

    /// Sizes the window in dependence to the screen geometry
    QSize sizeWindow(double width, double height)
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        QSize windowSize = screen->availableGeometry().size();
        windowSize.setWidth(windowSize.width() * width);
        windowSize.setHeight(windowSize.height() * height);

        return windowSize;
    }

    /// Returns data to move a widget to the screen center
    QPoint centerPosition(QWidget* parent)
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        return screen->geometry().center() - parent->rect().center();
    }

    /// Puts a string to clipboard
    void copy(QString copytext)
    {
        qDebug() << "Copy string...";
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(copytext);
    }

    /// Checks for an update - index 0: "false" = error, "noUpdates" = no updates available, else: new version | index 1: latestVersion
    QStringList getUpdateInformation()
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

    /// Searches for updates and returns information to the user
    void startUpdate(QWidget *parent, bool clearAppDir)
    {
        qDebug() << "Create tempAppDir and copy files...";

        QDirIterator dirIterator(QApplication::applicationDirPath(), QStringList{"*.*"}, QDir::Files, QDirIterator::Subdirectories);
        while (dirIterator.hasNext())
        {
            QString file = dirIterator.next();

            QDir().mkdir(QDir::tempPath() + "/OMSI-Tools_tempAppDir");

            QString newFilePath = QDir::tempPath() + "/OMSI-Tools_tempAppDir/" + file.remove(0, QApplication::applicationDirPath().count() + 1);
            QDir().mkpath(QFileInfo(newFilePath).absolutePath());

            qDebug().noquote() << "New path:" << newFilePath;
            if (QFile(newFilePath).exists())
                QFile(newFilePath).remove();

            qDebug().noquote() << "From path:" << QApplication::applicationDirPath() + "/" + file;
            qDebug() << "-----------------------------------------------------------------";
            QFile(QApplication::applicationDirPath() + "/" + file).copy(newFilePath);
        }
        qDebug() << "Finished.";

        QStringList args;
        args << "callFromMainApplication";
        args << QApplication::applicationDirPath();

        if (clearAppDir)
            args << "clearAppDir";

        if (!QProcess::startDetached(QDir::tempPath() + "/OMSI-Tools_tempAppDir/OMSI-Tools_Updater.exe", args))
        {
            qWarning() << "There was an error while starting the updater (with name 'OMSI-Tools_Updater.exe').";
            QMessageBox::warning(parent, QObject::tr("Updating %1").arg(OTName), QObject::tr("There was an error while starting the updater. Please retry it or contact the developer."));
        }
        else
        {
            qInfo() << "Start Updater...";
            QApplication::quit();
        }
    }
};

/// Class for file operations
class OTFileOperations
{
public:
    /// Returns an universal file header
    QString writeFileHeader()
    {
        return "File created with " + OTName + " " + OTVersion + " on " + misc.getDate() + ", " + misc.getTime() + "\n\n";
    }

    /// Checks if the backups folder exists
    void createBackupFolder()
    {
        if (!QDir("backup").exists())
            QDir().mkdir("backup");
    }

    /// Creates a shortcut
    void createShortcut(QString filepath, QString shortcutLocation, QWidget *parent)
    {
        if (QFile(filepath).link(shortcutLocation))
        {
            QMessageBox::information(parent, QObject::tr("Success"), QObject::tr("Successfully created shortcut!"));
            qInfo().noquote() << QString("Created shortcut in '%1'!").arg(shortcutLocation);
        }
        else
        {
            QMessageBox::warning(parent, QString(QObject::tr("Error")), QObject::tr("Could not create shortcut in %1.").arg(QDir().homePath() + "/Desktop"));
            qCritical().noquote() << QString("Could not create a shortcut in '%1'!").arg(shortcutLocation);
        }
    }

    /// Selects a folder or file in explorer
    void showInExplorer(QString absolutePath)
    {
        // ATTENTION: This will NOT work in the OneDrive folder - I don't know why.
        QStringList args;
        args << "/select," << QDir::toNativeSeparators(absolutePath);
        qDebug() << "Show in explorer:" << absolutePath;

        QProcess *process = new QProcess();
        process->start("explorer.exe", args);
    }

private:
    OTMiscellaneous misc;
};

/// Settings reader, writer etc.
class OTSettings
{
public:
    /// Writes a setting
    void write(QString module, QString name, QVariant value)
    {
        QSettings settings(OTName, module);
        settings.setValue(name, value);
        qDebug().noquote().nospace() << "Write settings to " << module << ": "<< name << ", value: " << value;
    }

    /// Reads a setting
    QVariant read(QString module, QString name, bool getInterpretedData = true)
    {
        QSettings settings(OTName, module);
        QVariant value = settings.value(name);
        qDebug().noquote().nospace() << "Read settings from " << module << ": "<< name << ", value: " << value;

        if (getInterpretedData && (name == "theme"))
            return getStyleSheet();
        else
            return value;
    }

    void remove(QString module, QString name)
    {
        QSettings(OTName, module).remove(name);
    }

    void removeAll()
    {
        QSettings("HKEY_CURRENT_USER\\SOFTWARE\\" + OTName, QSettings::NativeFormat).remove("");
    }

    /// Returns the whole stylesheet
    QString getStyleSheet()
    {
        QFile modularTheme(":/rec/data/themes/Modular.qss");

        if (!modularTheme.open(QFile::ReadOnly | QFile::Text))
            return "";

        if (!read("main\\themeData", "useStandardTheme").toBool())
        {
            QString theme = modularTheme.readAll();
            theme.replace("%<%Main%>%", read("main\\themeData", "Main").toString())
                             .replace("%<%MainSC%>%", read("main\\themeData", "MainSC").toString())
                             .replace("%<%Dis%>%", read("main\\themeData", "Dis").toString())
                             .replace("%<%DisD%>%", read("main\\themeData", "DisD").toString())
                             .replace("%<%Acc1%>%", read("main\\themeData", "Acc1").toString())
                             .replace("%<%Acc2%>%", read("main\\themeData", "Acc2").toString())
                             .replace("%<%Acc3%>%", read("main\\themeData", "Acc3").toString())
                             .replace("%<%Button%>%", read("main\\themeData", "Button").toString());
            qDebug() << theme;
            return theme;
        }
        else
        {
            qDebug() << "No theme!";
            return "";
        }
    }

    void setDefaultTheme(int theme)
    {
        // Standard
        if (theme == 0)
        {
            remove("main\\themeData", "");
            write("main\\themeData", "useStandardTheme", true);
        }
        // modernLight
        else if (theme == 1) // number ex. Cominear
        {
            remove("main\\themeData", "");
            write("main\\themeData", "useStandardTheme", false);
        }
        // modernDark (ex. Combinear)
        else if (theme == 2) // number ex. Darkeum
        {
            write("main\\themeData", "Main", "#3a3a3a");
            write("main\\themeData", "MainSC", "#262626");

            write("main\\themeData", "Dis", "#656565");
            write("main\\themeData", "DisD", "#404040");

            write("main\\themeData", "Acc1", "#111");
            write("main\\themeData", "Acc2", "#b78620");
            write("main\\themeData", "Acc3", "#fff");

            write("main\\themeData", "Button", "#525252");

            write("main\\themeData", "useStandardTheme", false);
        }
    }

    /// Select and control OMSI main dir path. Returns the OMSI path.
    QString getOmsiPath(QWidget *parent, QString path = "")
    {
        if (path == "")
            path = "C:/Program Files (x86)/Steam/steamapps/common/OMSI 2";

        QString mainDir = QFileDialog::getExistingDirectory(parent, QObject::tr("Select the OMSI main directory..."), path);

        if (mainDir != "" && !QFileInfo(QFile(mainDir + "/Omsi.exe")).exists())
        {
            qWarning().noquote() << "'" + mainDir + "' isn't an OMSI path!";
            QMessageBox::StandardButton reply = QMessageBox::warning(parent, QObject::tr("Could not found \"Omsi.exe\""), QObject::tr("%1 could not found in the selected directory. Is it the correct path? Otherwise, problems may appear in some modules. Should a new path be selected?").arg("'Omsi.exe'"), QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
                return getOmsiPath(parent);
        }

        return mainDir;
    }

    /// Gets all settings keys and its values
    QString getAllSettings()
    {
        QSettings set("HKEY_CURRENT_USER\\SOFTWARE\\" + OTName, QSettings::NativeFormat);

        QString returnString = "\n";

        foreach (QString current, set.allKeys())
            returnString += current + " -> " + set.value(current).toString() + "\n";

        return returnString;
    }

    /// Installs a translator
    void loadTranslator()
    {
        QTranslator *translator = new QTranslator();
        QTranslator *baseTranslator = new QTranslator();

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
            bool trInstall = qApp->installTranslator(translator);

            bool baseTrLoad = baseTranslator->load(baseLanguageFile, ":/rec/data/translations/");
            bool baseTrInstall = qApp->installTranslator(baseTranslator);

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

    /// Sets default needed settings
    void setDefaultSettings()
    {
        if (!read("main", "autoUpdateCheck").isValid())
            write("main", "autoUpdateCheck", 2);

        if (!read("main", "logfileMode").isValid())
            write("main", "logfileMode", 0);

        if (!read("main", "autosave").isValid())
            write("main", "autosave", true);

        if (!read("main", "autosaveDuration").isValid())
            write("main", "autosaveDuration", 15);

        if (!read("main", "confirmDeletion").isValid())
            write("main", "confirmDeletion", true);
    }
};

/// Message class
class OTMessage
{
public:
    /// Asks if the user wants to set the main dir
    bool setMainDir(QWidget *parent)
    {
        qDebug() << "Message: Action needs main directory. Set now (Yes/No)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("OMSI main directory not found"), QObject::tr("To continue the application needs the OMSI main directory. Should it be done now?"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            return false;

        return true;
    }

    /// universal unsaved-Message (with Save, Discard and Cancel). Returns 'save = 1', 'discard = 0' or 'cancel = -1'
    int unsavedContent(QWidget *parent)
    {
        qDebug() << "Message: Save unsaved content (Save/Discard/Cancel)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Unsaved content"), QObject::tr("There is unsaved content."), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
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

    /// universal unsaved-Message (with Yes and No). Returns 'Yes = 1' or 'No = 0'
    bool unsavedContentYesNo(QWidget *parent)
    {
        qDebug() << "Message: Save unsaved content (Yes/No)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Unsaved content"), QObject::tr("There is unsaved content. Do you want to save?"), QMessageBox::Yes | QMessageBox::No);
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

    /// If the user wants to open a file via a button and the file doesn't exist, this message es helpful
    void fileOpenError(QWidget *parent)
    {
        QMessageBox::information(parent, QObject::tr("Error while opening file"), QObject::tr("The selected file doesn't exists or is read-protected."));
    }

    /// Could not open a file from OMSI main dir
    void fileOpenErrorCloseOMSI(QWidget *parent, QString filename)
    {
        QMessageBox::critical(parent, QObject::tr("Error while opening file"), QString(QObject::tr("There was an error while opening '%1'. If OMSI is running, please close it and retry it. Furthermore, check if the file still exists.")).arg(filename));
    }

    /// Font module: No chars in font
    void noCharsInFont(QWidget *parent)
    {
        QMessageBox::warning(parent, QObject::tr("No chars in font"), QObject::tr("There are no chars in the font."));
    }

    /// Comfirm a deletion of anything
    bool confirmDeletion(QWidget *parent)
    {
        if (set.read("main", "confirmDeletion") == "false")
            return true;

        qDebug() << "Message: Confirm deletion (Yes/No)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Confirm deletion"), QObject::tr("Should the content be deleted?"), QMessageBox::Yes | QMessageBox::No);
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

    /// There was en error while saving a file
    void fileSaveError(QString filename, QWidget *parent = 0)
    {
        QMessageBox::warning(parent, QObject::tr("Save error"), QString(QObject::tr("There was an save error '%1'. If OMSI is running, please close it and retry it.")).arg(filename));
    }

    /// A module is due to narrow time deactivated
    void moduleDisabled(QWidget *parent = 0)
    {
        QMessageBox::information(parent, QObject::tr("Module still deactivated"), QObject::tr("Unfortunately, this module is still deactivated due to a time shortage in the developments. Please check for new updates in the next few days."));
    }

private:
    OTSettings set;
};

/// Calculates disk usage
class OTDiskUsage
{
public:
    /// Returns disk usage of directory 'dirPath'
    QString formatSize(QString dirPath)
    {
        qint64 size = calcDirSize(dirPath);

        int i;
        double outputSize = size;
        for(i = 0; i < units.size() - 1; i++)
        {
            if (outputSize < 1024)
                break;

            outputSize = outputSize / 1024;
        }

        return QString("%0 %1").arg(outputSize, 0, 'f', 2).arg(units[i]);;
    }

    /// Formats bytes values into other units if necessary
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
    /// Calculates size in bytes of a directory
    qint64 calcDirSize(QString dirPath)
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
            size += calcDirSize(dirPath + QDir::separator() + childDirPath);

        return size;
    }

    QStringList units = { QObject::tr("Bytes"),
                          QObject::tr("KB", "Short version of 'Kilobyte'"),
                          QObject::tr("MB", "Short version of 'Megabyte'"),
                          QObject::tr("GB", "Short version of 'Gigabyte'"),
                          QObject::tr("TB", "Short version of 'Terrabyte'"),
                          QObject::tr("PB", "Short version of 'Petabyte'")};
};

/// [STATIC] Class for some global needed strings
class OTStrings
{
public:
    /// Returns a translated month name
    static QString getMonthName(int monthNumber)
    {
        QMap<int, QString> months = {
            {1,  QObject::tr("January")},
            {2,  QObject::tr("February")},
            {3,  QObject::tr("March")},
            {4,  QObject::tr("April")},
            {5,  QObject::tr("May")},
            {6,  QObject::tr("June")},
            {7,  QObject::tr("July")},
            {8,  QObject::tr("August")},
            {9,  QObject::tr("September")},
            {10, QObject::tr("October")},
            {11, QObject::tr("November")},
            {12, QObject::tr("December")},
        };

        return months[monthNumber];
    }

    // Multilanguage strings
    static inline const QString langEn = "English";
    static inline const QString langDe = "Deutsch (German)";
    static inline const QString langFr = "Français (French)";
    static inline const QString langIt = "Italiano (Italian)";
    static inline const QString langCz = "Čeština (Czech)";

    // Fun facts (funFact, time in ms)
//    static inline const QList<QPair<QString, unsigned int>> funFacts = { QPair("English", 4),
//                                                                         QPair("English", 4),
//                                                                         (true) ? QPair("English", 4) :,
//                                                                       };
};

#endif // OTGLOBAL_H
