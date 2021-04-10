QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addserv.cpp \
    addtariff.cpp \
    addwindow.cpp \
    database.cpp \
    editwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    services.cpp

HEADERS += \
    addserv.h \
    addtariff.h \
    addwindow.h \
    database.h \
    editwindow.h \
    mainwindow.h \
    services.h

FORMS += \
    addserv.ui \
    addtariff.ui \
    addwindow.ui \
    editwindow.ui \
    mainwindow.ui \
    services.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
