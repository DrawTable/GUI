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

INCLUDEPATH += C:\opencv\release\install\include
LIBS += -L"C:\opencv\release\bin" -lopencv_calib3d300 -lopencv_core300 -lopencv_features2d300 -lopencv_flann300 -lopencv_highgui300 -lopencv_imgcodecs300 -lopencv_imgproc300 -lopencv_ml300 -lopencv_objdetect300 -lopencv_photo300 -lopencv_shape300 -lopencv_stitching300 -lopencv_superres300 -lopencv_video300 -lopencv_videoio300 -lopencv_videostab300

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
    camera/cameramanager.cpp \
    camera/cameramanagerupdater.cpp \
    modal/menudialog.cpp \
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
    camera/cameramanager.h \
    camera/cameramanagerupdater.h \
    modal/menudialog.h \
    tracking/leddetector.h

RESOURCES += \
    icons.qrc
