#ifndef OTPLATFORM_H
#define OTPLATFORM_H

#include <QCoreApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>
#include <QStringConverter>
#include <QVariant>
#include <QUrl>

/*!
    Everything which has to be done differently depending on the operating system.

    OMSI-Tools is primarily built for Windows, so the Windows branches keep the
    behaviour the application always had. The other branches try to get as close to it
    as the platform allows.
*/
class OTPlatform
{
public:
    /// Encoding of OMSI's configuration files. OMSI writes ANSI, which is what
    /// QStringConverter::System resolves to on a Windows machine - but not on Linux,
    /// where it would be UTF-8 and would garble every umlaut.
    static QStringConverter::Encoding omsiEncoding()
    {
#ifdef Q_OS_WIN
        return QStringConverter::System;
#else
        return QStringConverter::Latin1;
#endif
    }

    /// Absolute path of a file which lives next to the program binary.
    ///
    /// logfile.txt, _docs/, backup/ and texconv.exe used to be created through plain
    /// relative paths, i.e. in the working directory - while the very same files were
    /// always read back through applicationDirPath(). On Windows both are the same as
    /// long as the program is started by double click; on Linux the working directory
    /// is whatever the desktop environment hands over, usually the home directory.
    static QString applicationFile(const QString &relativePath)
    {
        return QCoreApplication::applicationDirPath() + "/" + relativePath;
    }

    /// Absolute path of the bundled DirectXTex converter, or an empty string if there
    /// is no build of it for this platform.
    static QString texconvExecutable()
    {
#ifdef Q_OS_WIN
        return applicationFile("texconv.exe");
#else
        return QString();
#endif
    }

    /// Path of the Qt maintenance tool which installs the updates.
    static QString updaterExecutable()
    {
#ifdef Q_OS_WIN
        return "../OMSI-Tools_Updater.exe";
#else
        return "../OMSI-Tools_Updater";
#endif
    }

    /// Opens the file manager and selects the given file or directory.
    static void showInFileManager(const QString &path)
    {
#ifdef Q_OS_WIN
        QStringList args;
        args << "/select," << QDir::toNativeSeparators(path);

        QProcess *process = new QProcess();
        process->start("explorer.exe", args);
#else
        // Dolphin, Nautilus and friends implement this interface; if none of them is
        // around, fall back to simply opening the containing directory.
        const QUrl url = QUrl::fromLocalFile(QFileInfo(path).absoluteFilePath());

        QProcess fileManager;
        fileManager.setStandardOutputFile(QProcess::nullDevice());
        fileManager.setStandardErrorFile(QProcess::nullDevice());
        fileManager.start("dbus-send", QStringList()
                          << "--session" << "--print-reply" << "--dest=org.freedesktop.FileManager1"
                          << "--type=method_call" << "/org/freedesktop/FileManager1"
                          << "org.freedesktop.FileManager1.ShowItems"
                          << "array:string:" + url.toString() << "string:");

        const bool shown = fileManager.waitForFinished(5000)
                           && (fileManager.exitStatus() == QProcess::NormalExit)
                           && (fileManager.exitCode() == 0);

        if (!shown)
        {
            qWarning() << "Could not select the file in the file manager - opening its directory instead.";
            QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(path).absolutePath()));
        }
#endif
    }

    /// Registered OMSI 2 installation, used as start directory of the path selection.
    static QString omsiInstallationGuess()
    {
#ifdef Q_OS_WIN
        return QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\aerosoft\\OMSI 2", QSettings::NativeFormat).value("Product_Path").toString();
#else
        // There is no registry, but OMSI is usually run through Steam / Proton.
        const QStringList candidates = {
            QDir::homePath() + "/.steam/steam/steamapps/common/OMSI 2",
            QDir::homePath() + "/.local/share/Steam/steamapps/common/OMSI 2",
            QDir::homePath() + "/.var/app/com.valvesoftware.Steam/.local/share/Steam/steamapps/common/OMSI 2"
        };

        for (const QString &current : candidates)
            if (QDir(current).exists()) return current;

        return QString();
#endif
    }

    /// Every stored preference, keyed as "module/name".
    static QList<QPair<QString, QVariant>> allPreferences(const QString &organisation)
    {
        QList<QPair<QString, QVariant>> preferences;

#ifdef Q_OS_WIN
        QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\" + organisation, QSettings::NativeFormat);

        for (const QString &current : settings.allKeys())
            preferences.append(QPair<QString, QVariant>(current, settings.value(current)));
#else
        // Without a registry QSettings keeps one file per module next to each other.
        const QDir directory(preferencesLocation(organisation), "*.conf");

        for (const QString &file : directory.entryList(QDir::Files))
        {
            const QString module = QFileInfo(file).completeBaseName();
            QSettings settings(organisation, module);

            for (const QString &current : settings.allKeys())
                preferences.append(QPair<QString, QVariant>(module + "/" + current, settings.value(current)));
        }
#endif

        return preferences;
    }

    /// Deletes every stored preference of the application.
    static void removeAllPreferences(const QString &organisation)
    {
#ifdef Q_OS_WIN
        QSettings("HKEY_CURRENT_USER\\SOFTWARE\\" + organisation, QSettings::NativeFormat).remove("");
#else
        QDir directory(preferencesLocation(organisation), "*.conf");
        for (const QString &current : directory.entryList(QDir::Files))
            directory.remove(current);
#endif
    }

private:
#ifndef Q_OS_WIN
    /// Directory QSettings stores the single module files in.
    static QString preferencesLocation(const QString &organisation)
    {
        return QFileInfo(QSettings(organisation, "main").fileName()).absolutePath();
    }
#endif
};

#endif // OTPLATFORM_H
