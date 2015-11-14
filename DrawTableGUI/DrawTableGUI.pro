#-------------------------------------------------
#
# Project created by QtCreator 2015-11-13T22:15:08
#
#-------------------------------------------------

QT       += core gui printsupport
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DrawTableGUI
TEMPLATE = app


SOURCES += main.cpp\
        core/mainwindow.cpp \
    view/table.cpp \
    controller/pencontroller.cpp \
    controller/dashcontroller.cpp \
    controller/rectanglecontroller.cpp \
    controller/ellipsecontroller.cpp \
    controller/generalcontroller.cpp

HEADERS  += core/mainwindow.h \
    view/table.h \
    controller/pencontroller.h \
    controller/dashcontroller.h \
    controller/rectanglecontroller.h \
    controller/abstractcontroller.h \
    controller/ellipsecontroller.h \
    controller/generalcontroller.h

RESOURCES += \
    icons.qrc
