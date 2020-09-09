TEMPLATE = lib
TARGET = server
CONFIG += dll
QT += core remoteobjects androidextras

SOURCES += \
    gpsserver.cpp

REPC_SOURCE += \
    gpsprovider.rep
