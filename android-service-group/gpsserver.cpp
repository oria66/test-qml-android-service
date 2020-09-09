#include <QAndroidService>
#include <QRemoteObjectHost>
#include "rep_gpsprovider_source.h"

class PingPong : public GpsProviderSource {
public slots:
    // PingPongSource interface
    void ping(const QString &msg) override {
        emit pong(msg + " from server");
    }
};

int main(int argc, char *argv[])
{
    QAndroidService app(argc, argv);

    QRemoteObjectHost srcNode(QUrl(QStringLiteral("local:replica")));
    PingPong pingPongServer;
    srcNode.enableRemoting(&pingPongServer);

    return app.exec();
}
