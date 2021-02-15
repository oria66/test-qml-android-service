#ifndef ANDROIDPERMISSIONS_H
#define ANDROIDPERMISSIONS_H

#include <QObject>
#include <QtAndroid>

class AndroidPermissions : public QObject
{
    Q_OBJECT
public:
    explicit AndroidPermissions(QObject *parent = nullptr);

    //    Q_INVOKABLE bool requestAndroidPermissions();

    Q_INVOKABLE void requestAndroidPermissions();

signals:
    void requestPermissions();
    void permissionsAccepted();
    void permissionsDenied();
};

#endif // ANDROIDPERMISSIONS_H
