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

INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab

SOURCES += main.cpp\
        core/mainwindow.cpp \
    view/table.cpp \
    controller/pencontroller.cpp \
    controller/dashcontroller.cpp \
    controller/rectanglecontroller.cpp \
    controller/ellipsecontroller.cpp \
    controller/generalcontroller.cpp \
    controller/erasercontroller.cpp \
    tracking/trackingmanager.cpp \
    tracking/screendetector.cpp \
    tracking/leddetector.cpp

HEADERS  += core/mainwindow.h \
    view/table.h \
    controller/pencontroller.h \
    controller/dashcontroller.h \
    controller/rectanglecontroller.h \
    controller/abstractcontroller.h \
    controller/ellipsecontroller.h \
    controller/generalcontroller.h \
    controller/erasercontroller.h \
    tracking/trackingmanager.h \
    tracking/screendetector.h \
    tracking/leddetector.h

RESOURCES += \
    icons.qrc
