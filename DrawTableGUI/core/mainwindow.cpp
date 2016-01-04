/**
 * Projet  :   DrawTable
 * Fichier :   MainWindow.cpp
 *
 * Auteurs :   Bron Sacha
 *             Pellet Marc
 *             Villa David
 *
 *
 * Description :
 *
 *
 */

#include "mainwindow.h"

#include "../tool/pencontroller.h"
#include "../tool/dashcontroller.h"
#include "../tool/rectanglecontroller.h"
#include "../tool/ellipsecontroller.h"
#include "../tool/erasercontroller.h"
#include "../drawing/drawingcontroller.h"
#include "../dialog/filedialog.h"
#include "menu.h"

#include <QApplication>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPixmap>
#include <QMenuBar>
#include <QColorDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent) {

    /* tool bar */
    toolBar = new ToolBar(this);
    connect(toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(updateToolBarActions(QAction*)));
    addToolBar(Qt::BottomToolBarArea, toolBar);

    /* drawing */

    QBrush bgColor(Qt::black);
    drawing = new Drawing(this);
    drawing->setBackgroundBrush(bgColor);
    drawing->setStyleSheet("border: 0px;");
    drawing->scene()->setSceneRect(0, 0, maximumWidth(), maximumHeight());

    /* drawing controller */
    controller = new DrawingController(drawing);

    /* window */

    setCursor(Qt::ArrowCursor);
    onPenTriggered(false);
    setCentralWidget(drawing);
    showFullScreen();
    menuBar()->hide();

    // initialisation de l'error manager et liaison des signaux
    errorManager = ErrorManager::getInstance();
    connect(errorManager, SIGNAL(cameraSelection()), this, SLOT(restartCameraSelection()));
    connect(errorManager, SIGNAL(quitApp()), this, SLOT(onQuitTriggered()));

    // Lancement du Camera Manager
    cm = CameraManager::getInstance();
    connect(cm, SIGNAL(cameraChoosen(int)), this, SLOT(onCameraChoosen(int)));

    launchMouseHandler();

    tryCameraMode();
}

MainWindow::~MainWindow() {
}

void MainWindow::launchMouseHandler() {
    QProcess* process = new QProcess;
    process->start("java Server");
    process->waitForStarted();
}

void MainWindow::tryCameraMode() {
    cm->initCameras();

    if (cm->countCameras() < 1) {
        QMessageBox::information(this, tr("No camera found"),
                                 tr("It seems you don't have any camera plugged or integrated.\n\
If you do have a camera, check if your OS recognizes it."));
        controller->enable();
    } else {
        QMessageBox::information(this, tr("Select your camera"),
            tr("When you will click on 'ok', your camera(s) will be displayed on a personnal windows.\n\
Please check the angle of the camera you want to use, so your working board is entirely in sight.\n\
To choose a camera, click on the chosen camera's screen into it's window."));

        cm->listCameras();
    }
}

// Lance le Tracking Manager une fois que l'utilisateur a choisi la caméra à utiliser
void MainWindow::onCameraChoosen(int cameraId) {
    QMessageBox::information(this, tr("Camera chosen"),
                             QString("You chose the camera #") +
                             QString::fromStdString(std::to_string(cameraId)) +
                             QString(".\nStart the calibration by clicking 'ok'. This operation shouldn't take more than few seconds.") +
                             QString(" Please don't move the camera or do anything while the calibration still running."));
    startTrackingManager(cameraId);
}

// Crée et démarre un thread qui gère le tracking du stylet
void MainWindow::startTrackingManager(int cameraId) {
    QThread* thread = new QThread;
    TrackingManager* worker = new TrackingManager(cameraId);
    worker->moveToThread(thread);

    // Lancement et arrêt du thread
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // Communication Main Window <--> Tracking Manager
    connect(worker, SIGNAL(showGreenScreen()), this, SLOT(onShowGreenScreen()));
    connect(this, SIGNAL(stratCalibration(int, int)), worker, SLOT(onStratCalibration(int, int)));
    connect(worker, SIGNAL(calibrationSuccess()), this, SLOT(onCalibrationSuccess()));
    connect(worker, SIGNAL(calibrationError(int)), errorManager, SLOT(onCalibrationError(int)));

    thread->start();
}

// Affiche un écran vert pour le calibrage
void MainWindow::onShowGreenScreen() {
    // afficher un écran vert pour le calibrage
    toolBar->hide();
    menuBar()->hide();

    QBrush bgColor(Qt::green);
    drawing->setBackgroundBrush(bgColor);

    // Lance le processus de calibration
    QRect rec = QApplication::desktop()->screenGeometry();
    emit stratCalibration(rec.width(), rec.height());
}

// Quand la calibration a réussie
void MainWindow::onCalibrationSuccess() {
    // force la main window en premier plan pour 'cacher les fenetres de la camera
    Qt::WindowFlags eFlags = windowFlags ();
    eFlags |= Qt::WindowStaysOnTopHint;
    setWindowFlags(eFlags);

    showFullScreen();
    toolBar->show();

    QBrush bgColor(Qt::black);
    drawing->setBackgroundBrush(bgColor);

    QMessageBox::information(this, tr("Calibration successful"),
                             tr("Calibration sucessful, the software is now ready to use !"));

    // on retire le flag pour éviter de la bloquer en premier plan
    eFlags &= ~Qt::WindowStaysOnTopHint;
    setWindowFlags(eFlags);
    showFullScreen();

    controller->enable();
}

// Quand la calibration a échouée
void MainWindow::onCalibrationError(int errorCode) {
    Q_UNUSED(errorCode)

    // TODO implementation
    toolBar->show();
    //menuBar()->show();

    QBrush bgColor(Qt::black);
    drawing->setBackgroundBrush(bgColor);
}

void MainWindow::updateToolBarActions(QAction* action) {
    if (action->isCheckable()){
        const QList<QAction*>& actions = toolBar->actions();
        foreach (QAction* a, actions) {
            if (a != action) { a->setChecked(false); }
        }
    }
}

void MainWindow::onPenTriggered(bool checked) {
    if (checked) { controller->setDrawController(PenController::getInstance()); }
    else { toolBar->getPen()->setChecked(true); }
    drawing->setCursor(QCursor(QPixmap(":/cursor/icons/pen.ico")));
}

void MainWindow::onDashTriggered(bool checked) {
    if (checked) { controller->setDrawController(DashController::getInstance()); }
    else { toolBar->getDash()->setChecked(true); }
    drawing->setCursor(Qt::CrossCursor);
}

void MainWindow::onEraserTriggered(bool checked) {
    if (checked) { controller->setDrawController(EraserController::getInstance()); }
    else { toolBar->getEraser()->setChecked(true); }
    drawing->setCursor(QCursor(QPixmap(":/cursor/icons/eraser.ico")));
}

void MainWindow::onEllipseTriggered(bool checked) {
    if (checked) { controller->setDrawController(EllipseController::getInstance()); }
    else { toolBar->getEllipse()->setChecked(true); }
    drawing->setCursor(Qt::CrossCursor);
}

void MainWindow::onRectangleTriggered(bool checked) {
    if (checked) { controller->setDrawController(RectangleController::getInstance()); }
    else { toolBar->getRectangle()->setChecked(true);}
    drawing->setCursor(Qt::CrossCursor);
}

void MainWindow::onMenuTriggered() {
    Menu dialog(this);
    dialog.exec();
}

void MainWindow::onSaveTriggered() {
    QString fileName = SystemFileDialog::getSaveFileName();
    // creation du conteneur
    QPixmap pixmap(drawing->width(), drawing->height());
    // creation du painter allant servir à effectuer notre rendu
    QPainter painter(&pixmap);
    // selection qualite
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    // generation du rendu
    drawing->render(&painter);
    // enregistrement
    pixmap.save(fileName);
    painter.end();
    controller->setToSave(false);
}

void MainWindow::onOpenTriggered() {
    if(controller->toSave()){
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setStyleSheet("QMessageBox { background: black; border: 2px solid #35322f; } QMessageBox QLabel {color: white; font: 18pt;} QMessageBox QPushButton{background: #35322f; color: white; height: 48px; width: 100px; font: 14pt;} QMessageBox QPushButton::hover{border: 2px solid white;}");
        msgBox.setWindowFlags(Qt::FramelessWindowHint);
        int ret = msgBox.exec();

        switch (ret) {
          case QMessageBox::Save:
              onSaveTriggered();
              openFile();
          case QMessageBox::Discard:
              openFile();
              break;
          case QMessageBox::Cancel:
              // Nothing Happend
              break;
          default:
              // should never be reached
              break;
        }

    }else{
        openFile();
    }
}

void MainWindow::onNewTriggered() {
    if(controller->toSave()){
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setStyleSheet("QMessageBox { background: black; border: 2px solid #35322f; } QMessageBox QLabel {color: white; font: 18pt;} QMessageBox QPushButton{background: #35322f; color: white; height: 48px; width: 100px; font: 14pt;} QMessageBox QPushButton::hover{border: 2px solid white;}");
        msgBox.setWindowFlags(Qt::FramelessWindowHint);
        int ret = msgBox.exec();

        switch (ret) {
          case QMessageBox::Save:
              onSaveTriggered();
              drawing->scene()->clear();
              controller->resetUndoHistory();
          case QMessageBox::Discard:
              drawing->scene()->clear();
              controller->resetUndoHistory();
              break;
          case QMessageBox::Cancel:
              // Nothing Happend
              break;
          default:
              // should never be reached
              break;
        }

    }else{
        drawing->scene()->clear();
        controller->resetUndoHistory();
    }
}

void MainWindow::onPrintTriggered() {
    QPrinter printer;
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        painter.setRenderHint(QPainter::Antialiasing);
        drawing->render(&painter);
        painter.end();
    }
}

void MainWindow::onUndoTriggered() {
    controller->undo();
}

void MainWindow::onRedoTriggered() {
    controller->redo();
}

void MainWindow::onThicknessChanged(QAction* action) {
    QPen* pen = controller->getPen();
    pen->setWidth(qvariant_cast<int>(action->data()));
}

void MainWindow::onQuitTriggered()
{
    if(controller->toSave()){
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setStyleSheet("QMessageBox { background: black; border: 2px solid #35322f; } QMessageBox QLabel {color: white; font: 18pt;} QMessageBox QPushButton{background: #35322f; color: white; height: 48px; width: 100px; font: 14pt;} QMessageBox QPushButton::hover{border: 2px solid white;}");
        msgBox.setWindowFlags(Qt::FramelessWindowHint);
        int ret = msgBox.exec();

        switch (ret) {
          case QMessageBox::Save:
              onSaveTriggered();
              QApplication::quit();
          case QMessageBox::Discard:
              QApplication::quit();
              break;
          case QMessageBox::Cancel:
              // Nothing Happend
              break;
          default:
              // should never been reached
              break;
        }

    }else{
        QApplication::quit();
    }
}

void MainWindow::openFile() {
    QString fileName = SystemFileDialog::getOpenFileName();
    QPixmap img(fileName);
    drawing->scene()->addPixmap(img);
}

void MainWindow::onColorTriggered(){
    QPen* pen = controller->getPen();
    QColorDialog* colorDialog = new QColorDialog(pen->color(), this);
    QColor selectedColor = colorDialog->getColor(pen->color(), this);
    pen->setColor(selectedColor);
}

void MainWindow::restartCameraSelection(){
    tryCameraMode();
}
