#include "androidpermissions.h"

AndroidPermissions::AndroidPermissions(QObject *parent) : QObject(parent)
{

}

void AndroidPermissions::requestAndroidPermissions()
{
    //Request requiered permissions at runtime

    const QVector<QString> permissions({"android.permission.ACCESS_COARSE_LOCATION",
                                        "android.permission.ACCESS_BACKGROUND_LOCATION",
                                        "android.permission.ACCESS_FINE_LOCATION"});

    for(const QString &permission : permissions){
        auto result = QtAndroid::checkPermission(permission);
        if(result == QtAndroid::PermissionResult::Denied){
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if(resultHash[permission] == QtAndroid::PermissionResult::Denied)
                emit permissionsDenied();
        }
    }

    emit permissionsAccepted();
}
