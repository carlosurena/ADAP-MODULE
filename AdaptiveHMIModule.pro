#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T21:59:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AdaptiveHMIModule
TEMPLATE = app


SOURCES += main.cpp\
        adaphmimainwindow.cpp \
    adaphmiwidgetstate.cpp \
    adaphmisaveload.cpp \
    adaphmisavelookuptable.cpp \
    adaphmixmlcreator.cpp \
    tinyxml2.cpp

HEADERS  += adaphmimainwindow.h \
    adaphmiwidgetstate.h \
    adaphmisaveload.h \
    adaphmisavelookuptable.h \
    adaphmixmlcreator.h \
    tinyxml2.h \
    stdint.h

FORMS    += adaphmimainwindow.ui

RESOURCES += \
    adapHMIModule.qrc
