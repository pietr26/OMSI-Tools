#ifndef OTNETWORKCONNECTION_H
#define OTNETWORKCONNECTION_H

#include <QtNetwork>
#include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QPair>
#include <QUrlQuery>
#include <QVariant>

class OTNetworkConnection : public QObject
{
    Q_OBJECT
public slots:
    /// [OVERLOADED] POST with file / Returns the downloaded file
    QByteArray post(const QUrl &url, QFile *file, QList<QPair<QString, QString>> params = QList<QPair<QString, QString>>(), unsigned int connectionTimeout = 10000)
    {
        lastSuccess = 0;
        return downloadPostFile(url, params, file, connectionTimeout);
    }

    /// [OVERLOADED] POST / Returns the downloaded file
    QByteArray post(const QUrl &url, QList<QPair<QString, QString>> params = QList<QPair<QString, QString>>(), unsigned int connectionTimeout = 10000)
    {
        lastSuccess = 0;
        return downloadPost(url, params, connectionTimeout);
    }

    /// [OVERLOADED] POST / Saves the download file to a local file
    int post(const QUrl &url, const QString filepath, QList<QPair<QString, QString>> params = QList<QPair<QString, QString>>(), unsigned int connectionTimeout = 10000)
    {
        lastSuccess = 0;
        saveToFile(filepath, downloadPost(url, params, connectionTimeout));
        return lastHttpCode;
    }

    /// [OVERLOADED] GET / Returns the downloaded file
    QByteArray get(const QUrl &url, unsigned int connectionTimeout = 10000)
    {
        lastSuccess = 0;
        return downloadGet(url, connectionTimeout);
    }

    /// [OVERLOADED] GET / Saves the download file to a local file
    int get(const QUrl &url, const QString filepath, unsigned int connectionTimeout = 10000)
    {
        lastSuccess = 0;
        saveToFile(filepath, downloadGet(url, connectionTimeout));
        return lastHttpCode;
    }

public:
    QNetworkReply *reply;
    int currentProgress = 0;
    int maxProgress = 0;
    int lastHttpCode = 0;

    /// Check if the last connection was successfully. -2: HTTP 503 | -1: No | 0: NULL | 1: Yes
    int lastSuccess = 0;

private slots:
    void downloadProgress(qint64 current, qint64 max)
    {
        currentProgress = current;
        maxProgress = max;
    }

private:
    QNetworkAccessManager manager;

    QByteArray downloadPost(const QUrl &url, QList<QPair<QString, QString>> params, unsigned int connectionTimeout)
    {
        qDebug().noquote().nospace() << "POST to '" << url.url() << "'";
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setTransferTimeout(connectionTimeout);

        QListIterator<QPair<QString, QString>> paramIterator(params);
        qDebug().noquote() << "URL params:" << params;

        QUrlQuery urlParams;

        while (paramIterator.hasNext())
        {
            QPair<QString, QString> param = paramIterator.next();
            urlParams.addQueryItem(param.first, param.second);
        }

        QEventLoop loop;

        connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = manager.post(request, urlParams.toString(QUrl::FullyEncoded).toUtf8());

        connect(reply, &QNetworkReply::downloadProgress, this, &OTNetworkConnection::downloadProgress);
        loop.exec();

        // Content length:
        // reply->header(QNetworkRequest::ContentLengthHeader).toInt();

        int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastHttpCode = httpCode;

        qDebug().noquote() << QString("POST finished (HTTP %1)").arg(httpCode);

        if (url.url().contains("omsi-tools.de") && (httpCode == 503))
        {
            lastSuccess = -2;
            qWarning().noquote() << QString("%1 is currently undergoing maintenance (HTTP 503). Please try again later.").arg(url.host());
        }
        else if ((httpCode >= 300) || (httpCode == 0))
        {
            lastSuccess = -1;
            return "";
        }
        else
            lastSuccess = 1;

        return reply->readAll();
    }

    QByteArray downloadPostFile(const QUrl &url, QList<QPair<QString, QString>> params, QFile *file, unsigned int connectionTimeout)
    {
        qDebug().noquote().nospace() << "POST to '" << url.url() << "'";
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
        request.setTransferTimeout(connectionTimeout);

        QListIterator<QPair<QString, QString>> paramIterator(params);
        qDebug().noquote() << "URL params:" << params;

        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\"; filename=\"" + QFileInfo(*file).fileName() + "\""));
        file->open(QFile::ReadOnly);
        imagePart.setBodyDevice(file);
        file->setParent(multiPart); // ownership transferred
        multiPart->append(imagePart);

        QEventLoop loop;

        connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = manager.post(request, multiPart);

        connect(reply, &QNetworkReply::downloadProgress, this, &OTNetworkConnection::downloadProgress);
        loop.exec();

        // Content length:
        // reply->header(QNetworkRequest::ContentLengthHeader).toInt();

        int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastHttpCode = httpCode;

        qDebug().noquote() << QString("POST finished (HTTP %1)").arg(httpCode);

        if (url.url().contains("omsi-tools.de") && (httpCode == 503))
        {
            lastSuccess = -2;
            qWarning().noquote() << QString("%1 is currently undergoing maintenance (HTTP 503). Please try again later.").arg(url.host());
        }
        else if ((httpCode >= 300) || (httpCode == 0))
        {
            lastSuccess = -1;
            return "";
        }
        else
            lastSuccess = 1;

        return reply->readAll();
    }

    QByteArray downloadGet(const QUrl &url, unsigned int connectionTimeout)
    {
        qDebug().noquote().nospace() << "GET to '" << url.url() << "'";
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setTransferTimeout(connectionTimeout);

        QEventLoop loop;

        connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = manager.get(request);

        connect(reply, &QNetworkReply::downloadProgress, this, &OTNetworkConnection::downloadProgress);
        loop.exec();

        // Content length:
        // reply->header(QNetworkRequest::ContentLengthHeader).toInt();

        int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastHttpCode = httpCode;

        if (url.url().contains("omsi-tools.de") && (httpCode == 503))
        {
            lastSuccess = -2;
            qWarning().noquote() << QString("%1 is currently undergoing maintenance (HTTP 503). Please try again later.").arg(url.host());
        }
        else if ((httpCode >= 300) || (httpCode == 0))
        {
            lastSuccess = -1;
            return "";
        }
        else
            lastSuccess = 1;

        return reply->readAll();
    }

    void saveToFile(const QString filepath, const QByteArray content)
    {
        QFile file(filepath);
        if (!file.open(QFile::WriteOnly))
            qWarning().noquote() << "Error while saving POST content!";
        else
            file.write(content);
    }
};

#endif // OTNETWORKCONNECTION_H
