#include "peoplemqttservice.h"

PeopleMqttService::PeopleMqttService(QObject *parent) : QObject(parent)
{
    client = new QMQTT::Client("wss://mqtt.sbytech.cl:8084/mqtt", "", QWebSocketProtocol::Version5);

    connect(client, &QMQTT::Client::connected, this, &PeopleMqttService::onConnected);
    connect(client, &QMQTT::Client::disconnected, this, &PeopleMqttService::onDisconnected);
    connect(client, &QMQTT::Client::subscribed, this, &PeopleMqttService::onSubscribed);
    connect(client, &QMQTT::Client::error, this, &PeopleMqttService::onMqttError);
    connect(client, &QMQTT::Client::pingresp, [=](){ qDebug() << "******************    MQTT Ping"; });
}

void PeopleMqttService::setConnection()
{
    qDebug() << "**********************    setConnection   / MQTT / ************";

    client->setVersion(QMQTT::MQTTVersion::V3_1_0);

    client->setClientId("Test_peoplePhone_" + QDateTime::currentDateTime().toString(Qt::ISODateWithMs));

    client->setUsername("groundster");
    client->setPassword("$$sby2019$$");

    client->setCleanSession(true);

    client->setAutoReconnect(true);
    client->setAutoReconnectInterval(2000);

    topics.insert("people", "/mauro/people/position");

    client->connectToHost();
}

void PeopleMqttService::stopConnection()
{
    client->disconnectFromHost();
    client->setAutoReconnect(false);
}

void PeopleMqttService::publishPeoppleInformation(double nort, double east)
{
    qDebug() << "Enviando mensajes";

    QJsonObject recordObject;
    recordObject.insert("uniqueId", "asdasdasdasd");
    recordObject.insert("dateTime", QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate));
    recordObject.insert("userName", "Name");
    recordObject.insert("company", "Comapny");

    QJsonObject addressObject;
    addressObject.insert("x", nort);
    addressObject.insert("y", east);

    recordObject.insert("pos", addressObject);

    /// Envio de datos normalmente cuando existe comunicacion
    QJsonDocument doc(recordObject);
    client->publish(QMQTT::Message(messageId++,topics.value("people"), doc.toJson(), 2));
}

void PeopleMqttService::onConnected()
{
    qDebug() << "*************   MQTT Connected";

    //client->subscribe(topics.value("people"),2);
}

void PeopleMqttService::onDisconnected()
{
    qDebug() << "*****************************   MQTT Disconnected";
}

void PeopleMqttService::onSubscribed(const QString &topic, const quint8 qos)
{
    Q_UNUSED(qos)
    qDebug() << "*************   MQTT Suscrito al topico: " << topic;
}

void PeopleMqttService::onMqttError(const QMQTT::ClientError error)
{
    qDebug() << "******************************   MQTT Error: " << error;
}
