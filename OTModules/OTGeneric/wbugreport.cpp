#include "wbugreport.h"
#include "ui_wbugreport.h"

wBugRepot::wBugRepot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wBugRepot)
{
    qInfo().noquote() << "Starting " + moduleName + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTName + " - " + tr("bug reports and feedback"));

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    qInfo().noquote() << moduleName + " started";
}

wBugRepot::~wBugRepot()
{
    delete ui;
}

/// Image upload finished
void wBugRepot::replyFinished(QNetworkReply *test)
{
    QString replyContent = test->readAll();

    QJsonObject json = QJsonDocument::fromJson(replyContent.toUtf8()).object();
    json = json.value("data").toObject();

    qDebug().noquote() << json.value("link").toString();

    QMessageBox::information(this, "Finished", replyContent);
    qDebug() << replyContent;
}

/// Closes the window
void wBugRepot::on_actionClose_triggered()
{
    close();
}

QHttpMultiPart *buildMultpart(const QVariantMap & data, const QMap<QString, QString> filenames){

    QHttpMultiPart *multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QVariantMap::const_iterator i_data = data.constBegin();
    while (i_data != data.constEnd()) {
        QHttpPart postpart;
        postpart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"").arg(i_data.key()));
        postpart.setBody(i_data.value().toByteArray());
        multipart->append(postpart);
        ++i_data;
    }
    QMap<QString, QString>::const_iterator i_filenames = filenames.constBegin();
    while (i_filenames != filenames.constEnd()) {

        QFile *file = new QFile(i_filenames.value());
        if(!file->open(QIODevice::ReadOnly)){
            delete  file;
            continue;
        }
        QHttpPart postpart;
        postpart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QString("form-data; name=\"%1\"; filename=\"%2\"")
                           .arg(i_filenames.key(), file->fileName()));
        postpart.setBodyDevice(file);
        multipart->append(postpart);
        file->setParent(multipart);
        ++i_filenames;
    }
    return multipart;
}

/// Sends the feeback
void wBugRepot::on_actionSend_triggered()
{
    qDebug() << "Send feeback...";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QNetworkRequest request(QUrl("https://api.imgur.com/3/image"));

    request.setRawHeader("Authorization", "Client-ID 5fce0514adfe193");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString imagePath = "C:/Users/pietr/Desktop/Testpicture.gif";
    QFile image(imagePath);

    QVariantMap data;

    if(image.open(QIODevice::ReadOnly))
    {
        data["image"] = image.readAll().toBase64();
        data["type"] = "base64";
        image.close();
    }

    QHttpMultiPart *multipart = buildMultpart(data, {});

    //params.addQueryItem("image", base64Image);
    //params.addQueryItem("type", "text/plain");

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    manager->post(request, multipart);
}



