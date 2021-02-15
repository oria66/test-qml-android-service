#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QAndroidIntent>
#include <QtAndroid>
#include <QAndroidService>
#include <QRemoteObjectNode>
#include <QtMath>

#include <QGeoPositionInfoSource>

#include "androidpermissions.h"

#include "rep_gpsprovider_replica.h"
#include "rep_gpsprovider_source.h"

class PingPong : public GpsProviderSimpleSource {
public slots:
    // PingPongSource interface

    PingPong(QObject *parent = nullptr): GpsProviderSimpleSource(parent)
    {
        source = QGeoPositionInfoSource::createDefaultSource(0);

        if (source)
        {
            connect(source, &QGeoPositionInfoSource::positionUpdated, [this](const QGeoPositionInfo &update)
            {
                emit loggingData(update.timestamp().toString(Qt::ISODate) + " " + update.coordinate().toString() + " from server");
            });
        }
    }

    void startLogging() override
    {
        emit loggingData("Entering Start Service from server");

        if (source)
        {
            source->setUpdateInterval(2000);
            source->startUpdates();
            setIsRunningService(true);
            emit loggingData("Start Service from server");
        }
        else
        {
            emit loggingData("Failed to start Service from server");
        }
    }

    void stopLogging() override
    {
        if (source)
        {
            source->stopUpdates();
            setIsRunningService(false);
            emit loggingData("Stop Service from server");
        }
        else
        {
            emit loggingData("Failed to stop Service from server");
        }
    }

    void ping(const QString &msg) override {
        emit loggingData(msg + " from server");
    }

     QGeoPositionInfoSource *source;
};

class ServiceHandler: public QObject{
public:
    ServiceHandler(QObject *parent=nullptr):QObject(parent){}

    Q_INVOKABLE void start()
    {
        QAndroidJniObject::callStaticMethod<void>("com/kdab/training/MyService",
                                                  "startMyService",
                                                  "(Landroid/content/Context;)V",
                                                  QtAndroid::androidActivity().object());
    }

    Q_INVOKABLE void stop()
    {
        QAndroidJniObject::callStaticMethod<void>("com/kdab/training/MyService",
                                                  "stopMyService",
                                                  "(Landroid/content/Context;)V",
                                                  QtAndroid::androidActivity().object());
    }

};

//#if defined (Q_OS_ANDROID)
//bool requestAndroidPermissions(){
//    //Request requiered permissions at runtime

//    const QVector<QString> permissions({"android.permission.ACCESS_COARSE_LOCATION",
//                                        "android.permission.ACCESS_BACKGROUND_LOCATION",
//                                        "android.permission.ACCESS_FINE_LOCATION"});

//    for(const QString &permission : permissions){
//        auto result = QtAndroid::checkPermission(permission);
//        if(result == QtAndroid::PermissionResult::Denied){
//            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
//            if(resultHash[permission] == QtAndroid::PermissionResult::Denied)
//                return false;
//        }
//    }

//    return true;
//}
//#endif

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        qDebug() << " Main activity execution";

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

        QGuiApplication app(argc, argv);

        QQmlApplicationEngine engine;

        ServiceHandler handler;

        // Starting the service

        handler.start();

        // Trying to connecting to source

        QRemoteObjectNode repNode;
        repNode.connectToNode(QUrl(QStringLiteral("local:replica")));
        QSharedPointer<GpsProviderReplica> rep(repNode.acquire<GpsProviderReplica>());
        bool res = rep->waitForSource();
        Q_ASSERT(res);

        QObject::connect(rep.data(), &GpsProviderReplica::loggingData, [](const QString &message){
            qDebug() << "Service sent: " << message;
        });
        rep->ping("Qt and Android are friends!");


#if defined (Q_OS_ANDROID)
    AndroidPermissions permissions;

    //permissions.requestAndroidPermissions();

    engine.rootContext()->setContextProperty("androidHandler", &permissions);

//    if(!requestAndroidPermissions())
//        return -1;
#endif
        // Load QML

        engine.rootContext()->setContextProperty("pingPong", rep.data());
        engine.rootContext()->setContextProperty("handler", &handler);

        const QUrl url(QStringLiteral("qrc:/main.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                         &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

        engine.load(url);
        return app.exec();
    }
    else if (argc > 1 && strcmp(argv[1], "-service") == 0)
    {
        qDebug() << "*****************************   Service starting with from the same .so file";
        QAndroidService app(argc, argv);

        QRemoteObjectHost srcNode(QUrl(QStringLiteral("local:replica")));
        PingPong pingPongServer;
        srcNode.enableRemoting(&pingPongServer);
        return app.exec();
    }
    else
    {
        qWarning() << "Unrecognized command line argument";
        return -1;
    }
}
