#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../drawing/drawing.h"
#include "../drawing/drawingcontroller.h"
#include "../tracking/trackingmanager.h"
#include "../tool/pencontroller.h"
#include "../tool/dashcontroller.h"
#include "../tool/rectanglecontroller.h"
#include "../tool/ellipsecontroller.h"
#include "../tool/erasercontroller.h"
#include "../camera/cameramanager.h"
#include "../error/errormanager.h"

#include <QApplication>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QProgressDialog>
#include <QPixmap>
#include <QColorDialog>
#include <QDebug>
#include <QRect>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QThread>
#include "toolbar.h"

class ToolBar;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

public slots:
    void onNewTriggered();
    void onOpenTriggered();
    void onSaveTriggered();
    void onPrintTriggered();
    void updateToolBarActions(QAction* action);
    void onPenTriggered(bool checked = false);
    void onDashTriggered(bool checked = false);
    void onEraserTriggered(bool checked = false);
    void onEllipseTriggered(bool checked = false);
    void onRectangleTriggered(bool checked = false);
    void onMenuTriggered();
    void onColorTriggered();
    void onUndoTriggered();
    void onRedoTriggered();
    void onThicknessChanged(QAction *action);
    void onQuitTriggered();
    void openFile();

    // Communication Main Window <--> Tracking Manager
    void onShowGreenScreen();
    void onCalibrationSuccess();
    void onCalibrationError(int errorCode);

    void onStylusCalibrationProgress(int value);
    void onStylusCalibrationSuccess();
    void onStylusCalibrationError(int errorCode);

    // Communication Main Window <--> Camera Manager
    void onCameraChoosen(int cameraId);

    // Communication Main Window <--> Error Manager
    void restartCameraSelection();

signals:
    void stratCalibration(int, int);
    void quitProg();

private:
    ToolBar* toolBar;
    Drawing* drawing;
    DrawingController* controller;
    ErrorManager* errorManager;
    CameraManager* cm;
    QProgressDialog* stylusCalibrationProgress;
    void startTrackingManager(int cameraId);
    void tryCameraMode();
};

#endif // MAINWINDOW_H
