#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../view/table.h"
#include "../controller/generalcontroller.h"
#include "../tracking/trackingmanager.h"
#include "../controller/pencontroller.h"
#include "../controller/dashcontroller.h"
#include "../controller/rectanglecontroller.h"
#include "../controller/ellipsecontroller.h"
#include "../controller/generalcontroller.h"
#include "../controller/erasercontroller.h"
#include "../camera/cameramanager.h"

#include <QApplication>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPixmap>
#include <QMenuBar>
#include <QColorDialog>
#include <QDebug>
#include <QRect>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QThread>

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
    void onModeTriggered();
    void onThicknessChanged();
    void onQuitTriggered();
    void openFile();

    // Communication Main Window <--> Tracking Manager
    void onShowGreenScreen();
    void onCalibrationSuccess();
    void onCalibrationError(int errorCode);

    // Communication Main Window <--> Camera Manager
    void onCameraChoosen(int cameraId);

signals:
    void stratCalibration(int, int);
    void quitProg();

private:
    QToolBar* toolBar;
    QToolButton* thickness;
    Table* table;

    QAction* menu;
    QAction* newImg;
    QAction* open;
    QAction* save;
    QAction* print;
    QAction* quit;
    QAction* pen;
    QAction* dash;
    QAction* eraser;
    QAction* ellipse;
    QAction* rectangle;
    QAction* undo;
    QAction* redo;
    QAction* color;
    QAction* mode;



    GeneralController* controller;

    void startTrackingManager(int cameraId);
};

#endif // MAINWINDOW_H
