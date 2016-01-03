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

/**
 * @brief Main class launching everything.
 *
 * This class is called by Qt when the application starts.
 * It creates the draw area and its ToolBar, the menu.
 * It also launches the calibration and receives its responses (success and errors).
 */
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

    // Communication Main Window <--> Camera Manager
    void onCameraChoosen(int cameraId);

    // Communication Main Window <--> Error Manager
    void restartCameraSelection();

signals:
    /**
     * Ask the TrackingManager to start the calibration process.
     */
    void stratCalibration(int, int);

private:
    ToolBar* toolBar;
    Drawing* drawing;
    DrawingController* controller;
    ErrorManager* errorManager;
    CameraManager* cm;

    void startTrackingManager(int cameraId);
    void tryCameraMode();
};

#endif // MAINWINDOW_H
