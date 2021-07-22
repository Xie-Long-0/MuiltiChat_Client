#include "RequestHandler.h"
#include "database/DatabaseManager.h"
#include "encryption/EncryptionManager.h"
#include <QJsonDocument>
#include <QJsonObject>

RequestHandler::RequestHandler(QTcpSocket *socket, QObject *parent)
    : QObject(parent)
    , clientSocket(socket)
{
    connect(clientSocket, &QTcpSocket::readyRead, this, &RequestHandler::onReadyRead);
}

void RequestHandler::start()
{
    // handle request
}

void RequestHandler::onReadyRead()
{
    QByteArray requestData = clientSocket->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(requestData);
    QJsonObject json = jsonDoc.object();

    qDebug() << clientSocket->peerAddress().toString() << "request" << json["type"].toString();

    QString username = json["username"].toString();
    QString encryptedPassword = json["password"].toString();

    DatabaseManager dbManager;
    if (dbManager.verifyUser(username, encryptedPassword))
    {
        QJsonObject response;
        response["status"] = "success";
        clientSocket->write(QJsonDocument(response).toJson());
        qDebug() << username << "login successful";
    }
    else
    {
        QJsonObject response;
        response["status"] = "failure";
        clientSocket->write(QJsonDocument(response).toJson());
        qDebug() << username << "login failed";
    }
}
