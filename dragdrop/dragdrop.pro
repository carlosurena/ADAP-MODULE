#-------------------------------------------------
#
# Project created by QtCreator 2017-02-08T11:14:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dragdrop
TEMPLATE = app


SOURCES += main.cpp\
        dragwidget.cpp \
    dropwidget.cpp \
    selector.cpp \
    dropwidget.cpp \


HEADERS  += dragwidget.h \
    ../selector.h

FORMS    += dragwidget.ui

RESOURCES += \
    draggableicons.qrc
