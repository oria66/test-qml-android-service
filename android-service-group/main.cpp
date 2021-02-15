#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>
#include <QRemoteObjectNode>

#include "rep_gpsprovider_replica.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Starting the service

    QAndroidJniObject::callStaticMethod<void>("com/kdab/training/MyService",
                                              "startMyService",
                                              "(Landroid/content/Context;)V",
                                              QtAndroid::androidActivity().object());

    // Trying to connecting to source

    QRemoteObjectNode repNode;
    repNode.connectToNode(QUrl(QStringLiteral("local:replica")));
    QSharedPointer<GpsProviderReplica> rep(repNode.acquire<GpsProviderReplica>());
    bool res = rep->waitForSource();
    Q_ASSERT(res);

    QObject::connect(rep.data(), &GpsProviderReplica::pong, [](const QString &message){
        qDebug() << "Service sent: " << message;
    });
    rep->ping("Qt and Android are friends!");


    // Load QML

    engine.rootContext()->setContextProperty("pingPong", rep.data());

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
