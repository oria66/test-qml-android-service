TEMPLATE = lib
TARGET = serverService
CONFIG += dll
QT += core remoteobjects androidextras

SOURCES += \
    gpsserver.cpp

REPC_SOURCE += \
    gpsprovider.rep
