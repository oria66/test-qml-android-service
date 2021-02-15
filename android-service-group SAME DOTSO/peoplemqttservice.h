#ifndef PEOPLEMQTTSERVICE_H
#define PEOPLEMQTTSERVICE_H

#include <QObject>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimerEvent>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "qmqtt.h"

class PeopleMqttService : public QObject
{
    Q_OBJECT
public:
    explicit PeopleMqttService(QObject *parent = nullptr);
    void setConnection();
    void stopConnection();
    void publishPeoppleInformation(double nort, double east);

public slots:
    void onConnected();
    void onDisconnected();
    void onSubscribed(const QString& topic, const quint8 qos = 0);
    void onMqttError(const QMQTT::ClientError error);

private:
    QHash<QString, QString> topics;
    QMQTT::Client *client;

    quint16 messageId{};
};

#endif // PEOPLEMQTTSERVICE_H
