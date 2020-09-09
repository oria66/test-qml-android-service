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

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QAndroidJniObject::callStaticMethod<void>("com/kdab/training/MyService",
                                              "startMyService",
                                              "(Landroid/content/Context;)V",
                                              QtAndroid::androidActivity().object());

    QRemoteObjectNode repNode;
    repNode.connectToNode(QUrl(QStringLiteral("local:replica")));
    QSharedPointer<GpsProviderReplica> rep(repNode.acquire<GpsProviderReplica>());
    engine.rootContext()->setContextProperty("pingPong", rep.data());
    bool res = rep->waitForSource();
    Q_ASSERT(res);
    rep->ping("Hello");

    return app.exec();
}
