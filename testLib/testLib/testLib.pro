#-------------------------------------------------
#
# Project created by QtCreator 2019-04-15T15:40:13
#
#-------------------------------------------------

QT       += core gui sql printsupport

#CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testLib
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    calprointerval.cpp \
    recordmanager.cpp \
    qcustomplot.cpp

HEADERS += \
        mainwindow.h \
    Includes/Buffer.h \
    Includes/ModbusApi.h \
    Includes/MsgPackage.h \
    Includes/resource.h \
    Includes/Util.h \
    calprointerval.h \
    recordmanager.h \
    Log.h \
    common.h \
    qcustomplot.h \
    modbus/modbus.h \
    modbus/modbus-private.h \
    modbus/modbus-rtu.h \
    modbus/modbus-rtu-private.h \
    modbus/modbus-tcp.h \
    modbus/modbus-tcp-private.h \
    modbus/modbus-version.h

FORMS += \
        mainwindow.ui

CONFIG(debug, debug|release){
    contains(QT_ARCH, i386) {
        LIBS += -L$$PWD/Libraries/x86/debug/ -lmodbus
        LIBS += -L$$PWD/Libraries/x86/debug/ -lmodbusApi
    }else {
        LIBS += -L$$PWD/Libraries/x64/debug/ -lmodbusApi
    }
} else {
    contains(QT_ARCH, i386) {
        LIBS += -L$$PWD/Libraries/x86/release/ -lmodbusApi
    }else {
        LIBS += -L$$PWD/Libraries/x64/release/ -lmodbusApi
    }
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
