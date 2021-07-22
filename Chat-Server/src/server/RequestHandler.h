#pragma once

#include <QObject>
#include <QTcpSocket>

class RequestHandler : public QObject
{
    Q_OBJECT

public:
    explicit RequestHandler(QTcpSocket *socket, QObject *parent = nullptr);
    void start();

private slots:
    void onReadyRead();

private:
    QTcpSocket *clientSocket;
};
