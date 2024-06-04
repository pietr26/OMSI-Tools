#include "lfclientapiinterface.h"

LFClientAPIInterface::LFClientAPIInterface(QObject *parent) :
    QObject(parent),
    m(new QNetworkAccessManager(this)) {
}

bool LFClientAPIInterface::isLoggedIn() {
    return !_currentToken.isEmpty();
}

LFClientAPIInterface::ErrorType LFClientAPIInterface::errorType() const {return _errorType;}
QString LFClientAPIInterface::errorString() const {return _errorString;}
QString LFClientAPIInterface::currentToken() const {return _currentToken;}

bool LFClientAPIInterface::login(const QString &username, const QString &password) {
    QNetworkRequest req = createNewRequest(Login);
    req.setRawHeader("Authorization", "Basic " + QString(username + ":" + password).toLocal8Bit().toBase64());
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();

    bool ok;
    QJsonObject obj = handleReply(r, &ok);
    if(ok) {
        _currentToken  = obj.value("token").toString();
        _currentUserID = obj.value("user_id").toString();
        emit loginStatusChanged(true);
        qInfo() << "LFClient: Login successful.";
    } else qInfo() << "LFClient: Login failed.";

    return ok;
}

bool LFClientAPIInterface::logout() {
    QNetworkRequest req = createNewRequest(Logout);
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();

    bool ok;
    QJsonObject obj = handleReply(r, &ok);
    if(ok) {
        _currentToken = "";
        emit loginStatusChanged(false);
        qInfo() << "LFClient: Logout successful.";
    } else qInfo() << "LFClient: Logout failed.";
    return ok;
}

LFCApiGlobalData LFClientAPIInterface::getGlobalData()
{
    qInfo() << "LFClient: get global data...";
    QNetworkRequest req = createNewRequest(GetGlobalData);
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();

    bool ok;
    QJsonObject obj = handleReply(r, &ok);
    qDebug().noquote() << "json:" << obj;
    if(ok) {
        return LFCApiGlobalData(obj);
    } else
        return LFCApiGlobalData();
}

QList<LFCApiNotification> LFClientAPIInterface::getNotifications()
{
    qInfo() << "LFClient: get notifications...";
    QNetworkRequest req = createNewRequest(GetNotifications);
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();

    bool ok;
    QJsonObject obj = handleReply(r, &ok);
    qDebug().noquote() << "json:" << obj;
    QList<LFCApiNotification> result;
    if(ok) {
        QJsonArray arr = obj.value("results").toArray();
        for(int i = 0; i < arr.count(); i++)
            result << LFCApiNotification(arr.at(i).toObject());
    }
    return result;
}

QList<LFCApiTrip> LFClientAPIInterface::getMyTrips()
{
    qInfo() << "LFClient: get trips...";
    QNetworkRequest req = createNewRequest(GetMyTrips);
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();

    bool ok;
    QJsonObject obj = handleReply(r, &ok);
    qDebug().noquote() << "json:" << obj;
    QList<LFCApiTrip> result;
    if(ok) {
        QJsonArray arr = obj.value("results").toArray();
        for(int i = 0; i < arr.count(); i++)
            result << LFCApiTrip(arr.at(i).toObject());
    }
    return result;
}

LFCApiVehicle LFClientAPIInterface::getVehicle(const int &id)
{
    qInfo() << "LFClient: get vehicle...";
    QList<QPair<QString, QString>> params;
    params << QPair<QString, QString>("id", QString::number(id));

    QNetworkRequest req = createNewRequest(GetVehicle, params);
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();

    bool ok;
    QJsonObject obj = handleReply(r, &ok);
    qDebug().noquote() << "json:" << obj;
    LFCApiVehicle result;
    if(ok)  return LFCApiVehicle(obj);
    else return LFCApiVehicle();
}

QList<LFCApiVehicle> LFClientAPIInterface::getAllVehicles()
{
    qInfo() << "LFClient: get vehicles...";
    QNetworkRequest req = createNewRequest(GetAllVehicles);
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();

    bool ok;
    QJsonObject obj = handleReply(r, &ok);
    qDebug().noquote() << "json:" << obj;
    QList<LFCApiVehicle> result;
    if(ok) {
        QJsonArray arr = obj.value("results").toArray();
        for(int i = 0; i < arr.count(); i++)
            result << LFCApiVehicle(arr.at(i).toObject());
    }
    return result;
}

LFCApiSpeakRequest LFClientAPIInterface::getMySpeakRequest()
{
    qInfo() << "LFClient: get own speak request...";

    QNetworkRequest req = createNewRequest(GetMySpeakRequest);
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();

    bool ok;
    QJsonObject obj = handleReply(r, &ok);
    qDebug().noquote() << "json:" << obj;
    LFCApiVehicle result;
    if(ok)  return LFCApiSpeakRequest(obj.value("value").toInt());
    else return LFCApiSpeakRequest();
}

void LFClientAPIInterface::requestSpeak(LFCApiSpeakRequest request)
{
    qInfo() << "LFClient: send speak request...";
    QList<QPair<QString, QString>> params;
    params << QPair<QString, QString>("speakRequest", QString::number(request));

    QNetworkRequest req = createNewRequest(RequestSpeak, params);
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();
}

QNetworkRequest LFClientAPIInterface::createNewRequest(const ApiEndpoint &endpoint, const QList<QPair<QString, QString>> &parameters) const {
    QString url = "https://backend.omsi-tools.de/api/lfClient/v1/";
    switch(endpoint) {
        case AnswerSpeakRequest: url += "answerSpeakRequest.php";  break;
        case GetAllParticipants: url += "getAllParticipants.php";  break;
        case GetAllSpeakRequest: url += "getAllSpeakRequests.php"; break;
        case GetGlobalData:      url += "getGlobalData.php";       break;
        case GetNotifications:   url += "getNotifications.php";    break;
        case GetParticipant:     url += "getParticipant.php";      break;
        case Login:              url += "login.php";               break;
        case Logout:             url += "logout.php";              break;
        case RequestSpeak:       url += "requestSpeak.php";        break;
        case UpdateUserStatus:   url += "updateUserStatus.php";    break;
        case GetTrips:           url += "getTrips.php";            break;
        case GetMyTrips:         url += "getMyTrips.php";          break;
        case GetVehicle:         url += "getVehicle.php";          break;
        case GetAllVehicles:     url += "getAllVehicles.php";      break;
        case GetMySpeakRequest:  url += "getMySpeakRequest.php";   break;
    };

    qDebug().noquote() << "LFClient: request to" << url;
    emit requestRequested();

    QStringList parameterString;
    for(int i = 0; i < parameters.count(); ++i) {
        QPair<QString, QString> current = parameters[i];
        parameterString << current.first + "=" + current.second;
    }

    qDebug().noquote() << "params:" << parameterString;

    if(!parameterString.isEmpty())
        url += "?" + parameterString.join("&");

    QNetworkRequest req((QUrl(url)));
    if(endpoint != Login)
        req.setRawHeader("Authorization", "Bearer " + _currentToken.toLocal8Bit());

    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    req.setAttribute(QNetworkRequest::CookieSaveControlAttribute, QNetworkRequest::Manual);

    req.setRawHeader(QByteArray("Host"), QByteArray("backend.omsi-tools.de"));
    req.setRawHeader(QByteArray("User-Agent"), QByteArray("OMSI-Tools/" + OTInformation::versions::currentVersion.first.toUtf8()));

    return req;
}

QJsonObject LFClientAPIInterface::handleReply(QNetworkReply *r, bool *ok, int *httpCode) {
    const int statusCode = r->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug().noquote() << "LFClient: request status" << statusCode;

    if(httpCode)
        *httpCode = statusCode;

    if(ok)
        *ok = statusCode == 200;

    QJsonParseError err;
    QJsonObject obj = QJsonDocument::fromJson(r->readAll(), &err).object();
    if(err.error != QJsonParseError::NoError) {
        // error parsing json
        if(ok) *ok = false;
        qCritical() << "failed parsing json: " << err.errorString();
        return QJsonObject();
    } else {
        if(statusCode != 200) {
            const QString errorName = obj.value("error").toString();
            if(errorName == "login_invalid") _errorType = LoginInvalid;
            else if(errorName == "user_not_found") _errorType = LoginUserNotFound;
            else if(errorName == "wrong_password") _errorType = LoginWrongPassword;
            else if(errorName == "unauthorized") _errorType = Unauthorized;
            else if(errorName == "access_denied") _errorType = AccessDenied;
            else if(errorName == "invalid_url_parameters") _errorType = InvalidParams;
            else if(errorName == "sql_request_error") _errorType = SqlRequestError;
            else if(errorName == "user_not_found") _errorType = UserNotFound;
            else if(errorName == "speak_request_already_exists") _errorType = SpeakRequestAlreadyExists;
            else if(errorName == "speak_request_does_not_exists") _errorType = SpeakRequestDoesNotExist;
            _errorString = obj.value("error_description").toString();
        }
    }

    return obj;
}
