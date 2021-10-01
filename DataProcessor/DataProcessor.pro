TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        conqueue.cpp \
        dataanalizer.cpp \
        location.cpp \
        main.cpp \
        parser.cpp \
        sessionmanager.cpp

INCLUDEPATH += "$PWD/paho/headers/mqtt"
LIBS += -lpthread
LIBS += -lpaho-mqtt3as
LIBS += -lpaho-mqttpp3

HEADERS += \
    conqueue.h \
    dataanalizer.h \
    location.h \
    parser.h \
    pch.h \
    primitives.h \
    sessionmanager.h
