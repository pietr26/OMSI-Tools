#ifndef OTMISCELLANEOUS_H
#define OTMISCELLANEOUS_H

#include <QApplication>
#include <QClipboard>
#include <QDate>
#include <QDebug>
#include <QGuiApplication>
#include <QPixmap>
#include <QProcess>
#include <QScreen>
#include <QTime>
#include <QWidget>

#include "OTPlatform.h"

class OTMiscellaneous
{
public:
    void openInExplorer(QString path)
    {
        OTPlatform::showInFileManager(path);
    }

    void restart()
    {
        qInfo() << "Restart application...";
        qApp->quit();
        QProcess::startDetached(qApp->applicationFilePath(), qApp->arguments().mid(1));
    }

    QString getTime(QString format = "hh:mm:ss") { return QTime::currentTime().toString(format); }

    QString getDate(QString format = "dd.MM.yyyy") { return QDate::currentDate().toString(format); }

    /// Sizes the window in relation to the screen geometry
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

    void copy(QString copytext)
    {
        qDebug() << "Copy string...";
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(copytext);
    }

    void copy(QPixmap copyPixmap)
    {
        qDebug() << "Copy image...";
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setPixmap(copyPixmap);
    }
};

#endif // OTMISCELLANEOUS_H
