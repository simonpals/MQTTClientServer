TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "$PWD/paho/headers/mqtt"
LIBS += -lpthread
LIBS += -lpaho-mqtt3as
LIBS += -lpaho-mqttpp3

SOURCES += \
        locationinfo.cpp \
        main.cpp \
        mqttservice.cpp \
        trajectory.cpp

HEADERS += \
    locationinfo.h \
    mqttservice.h \
    pch.h \
    primitives.h \
    sessioninfo.h \
    trajectory.h
