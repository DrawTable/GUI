#include "mainwindow.h"

#include "../controller/pencontroller.h"
#include "../controller/dashcontroller.h"
#include "../controller/rectanglecontroller.h"
#include "../controller/ellipsecontroller.h"
#include "../controller/generalcontroller.h"
#include "../controller/erasercontroller.h"
#include "../dialog/menudialog.h"
#include "../dialog/filedialog.h"

#include <QApplication>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPixmap>
#include <QMenuBar>
#include <QColorDialog>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent) {
    // Creation de la vue
    QBrush bgColor(Qt::black);
    table = new Table(this);
    table->setBackgroundBrush(bgColor);
    table->setStyleSheet("border: 0px;");
    table->scene()->setSceneRect(0, 0, maximumWidth(), maximumHeight());

    // Creation des actions de la toolbar
    menu = new QAction(this);
    menu->setIcon(QIcon(":/tool/icons/menu.png"));
    connect(menu, SIGNAL(triggered()), this, SLOT(onMenuTriggered()));

    pen = new QAction(this);
    pen->setCheckable(true);
    pen->setChecked(true);
    pen->setIcon(QIcon(":/tool/icons/pen.png"));
    connect(pen, SIGNAL(triggered(bool)), this, SLOT(onPenTriggered(bool)));

    undo = new QAction(this);
    undo->setIcon(QIcon(":/tool/icons/undo.png"));
    connect(undo, SIGNAL(triggered()), this, SLOT(onUndoTriggered()));

    redo = new QAction(this);
    redo->setIcon(QIcon(":/tool/icons/redo.png"));
    connect(redo, SIGNAL(triggered()), this, SLOT(onRedoTriggered()));

    dash = new QAction(this);
    dash->setCheckable(true);
    dash->setIcon(QIcon(":/tool/icons/dash.png"));
    connect(dash, SIGNAL(triggered(bool)), this, SLOT(onDashTriggered(bool)));

    eraser = new QAction(this);
    eraser->setCheckable(true);
    eraser->setIcon(QIcon(":/tool/icons/eraser.png"));
    connect(eraser, SIGNAL(triggered(bool)), this, SLOT(onEraserTriggered(bool)));

    ellipse = new QAction(this);
    ellipse->setCheckable(true);
    ellipse->setIcon(QIcon(":/tool/icons/ellipse.png"));
    connect(ellipse, SIGNAL(triggered(bool)), this, SLOT(onEllipseTriggered(bool)));

    rectangle = new QAction(this);
    rectangle->setCheckable(true);
    rectangle->setIcon(QIcon(":/tool/icons/rectangle.png"));
    connect(rectangle, SIGNAL(triggered(bool)), this, SLOT(onRectangleTriggered(bool)));

    color = new QAction(this);
    color->setIcon(QIcon(":/tool/icons/color.png"));
    connect(color, SIGNAL(triggered()), this, SLOT(onColorTriggered()));

    QList<QIcon> icons;
    for (int i = 0; i < 6; ++i) {
        icons << QIcon(QString(":/tool/icons/thickness%1.png").arg(i + 1));
    }

    QStringList thicknesses;
    thicknesses << "1 px"  << "3 px" << "5 px"  << "7 px" << "10 px" << "15 px";

    QMenu* thicknessMenu = new QMenu(this);
    for (int i = 0; i < thicknesses.count(); ++i) {
        QAction *action = new QAction(icons.at(i), thicknesses.at(i), this);
        action->setData(thicknesses.at(i).split(" ")[0].toInt());
        action->setCheckable(true);
        connect(action, SIGNAL(triggered()), this, SLOT(onThicknessChanged()));
        thicknessMenu->addAction(action);
    }
    thicknessMenu->actions().first()->setChecked(true);
    thicknessMenu->setStyleSheet("background: rgb(46,46,46);");

    thickness = new QToolButton(this);
    thickness->setIcon(QIcon(":/tool/icons/thickness.png"));
    thickness->setPopupMode(QToolButton::InstantPopup);
    thickness->setMenu(thicknessMenu);
    thickness->setStyleSheet("QToolButton::menu-indicator{image: none;}");

    // Creation de la toolbar et integration des actions
    QWidget* spacer1 = new QWidget();
    spacer1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QWidget* spacer2 = new QWidget();
    spacer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar = new QToolBar(tr("Tools"));
    toolBar->addAction(menu);
    toolBar->addWidget(spacer1);
    toolBar->addAction(pen);
    toolBar->addAction(eraser);
    toolBar->addSeparator();
    toolBar->addWidget(thickness);
    toolBar->addSeparator();
    toolBar->addAction(undo);
    toolBar->addAction(redo);
    toolBar->addSeparator();
    toolBar->addAction(dash);
    toolBar->addAction(ellipse);
    toolBar->addAction(rectangle);
    toolBar->addSeparator();
    toolBar->addAction(color);
    toolBar->addWidget(spacer2);
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setStyleSheet("QToolBar{ background: rgb(46,46,46); border: 0px; }");
    connect(toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(updateToolBarActions(QAction*)));
    addToolBar(Qt::BottomToolBarArea, toolBar);
    onPenTriggered(false);

    // Creation du controleur principale
    controller = new GeneralController(table);

    // Integration de la vue a la GUI
    setCentralWidget(table);

    showFullScreen();
    menuBar()->hide();

    tryCameraMode();
}

MainWindow::~MainWindow() {
}

void MainWindow::tryCameraMode() {
    // Lancement du Camera Manager
    CameraManager* cm = CameraManager::getInstance();
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
        connect(cm, SIGNAL(cameraChoosen(int)), this, SLOT(onCameraChoosen(int)));
    }
}

// Lance le Tracking Manager une fois que l'utilisateur a choisi la caméra à utiliser
void MainWindow::onCameraChoosen(int cameraId) {
    QMessageBox::information(this, tr("Camera chosen"),
                             QString("You chose the camera #") +
                             QString::fromStdString(std::to_string(cameraId)) +
                             QString(".\n Start the calibration by clicking 'ok'. This operation shouldn't take more than few seconds.") +
                             QString("Please don't move the camera or do anything while the calibration still running."));
    startTrackingManager(cameraId);
}

// Crée et démarre un thread qui gère le tracking du stylet
void MainWindow::startTrackingManager(int cameraId) {
    QThread* thread = new QThread;
    worker = new TrackingManager(cameraId);
    worker->moveToThread(thread);

    // Lancement et arrêt du thread
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // Communication Main Window <--> Tracking Manager
    connect(this, SIGNAL(stratCalibration(int, int)), worker, SLOT(onStratCalibration(int, int)));
    connect(worker, SIGNAL(showGreenScreen()), this, SLOT(onShowGreenScreen()));
    connect(worker, SIGNAL(calibrationSuccess()), this, SLOT(onCalibrationSuccess()));
    connect(worker, SIGNAL(calibrationError(int)), this, SLOT(onCalibrationError(int)));

    // stylus calibration
    connect(worker, SIGNAL(stylusCalibrationSuccess()), this, SLOT(onStylusCalibrationSuccess()));
    connect(worker, SIGNAL(stylusCalibrationError(int)), this, SLOT(onStylusCalibrationError(int)));
    connect(worker, SIGNAL(stylusCalibrationProgress(int)), this, SLOT(onStylusCalibrationProgress(int)));

    thread->start();
}


void MainWindow::onStylusCalibrationSuccess(){
    qDebug()  << "[MainWindow] stylus calibration Success";
}
void MainWindow::onStylusCalibrationError(int errorCode){
    qDebug()  << "[MainWindow] stylus calibration Error: " << errorCode;
}

void MainWindow::onStylusCalibrationProgress(int value){
    stylusCalibrationProgress->setValue(value);
}


// Affiche un écran vert pour le calibrage
void MainWindow::onShowGreenScreen() {
    // afficher un écran vert pour le calibrage
    toolBar->hide();
    menuBar()->hide();

    QBrush bgColor(Qt::green);
    table->setBackgroundBrush(bgColor);

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
    //showFullScreen();

    toolBar->show();
    //menuBar()->show();

    QBrush bgColor(Qt::black);
    table->setBackgroundBrush(bgColor);

    QMessageBox::information(this, tr("Calibration successfull"),
                             tr("Calibration sucessfull, the software is now ready to use !"));

    // on retire le flag pour éviter de la bloquer en premier plan
    eFlags &= ~Qt::WindowStaysOnTopHint;
    setWindowFlags(eFlags);
    showFullScreen();
    controller->enable();

    stylusCalibrationProgress = new QProgressDialog("Please draw something with your stylus", "cancel", 0, 100, this);
    QTimer* t = new QTimer(this);
    connect(t, SIGNAL(timeout()), worker, SLOT(onStartStylusCalibration()));
    t->start(0);
    stylusCalibrationProgress->show();
}

// Quand la calibration a échouée
void MainWindow::onCalibrationError(int errorCode) {
    Q_UNUSED(errorCode)

    // TODO implementation
    toolBar->show();
    //menuBar()->show();

    QBrush bgColor(Qt::black);
    table->setBackgroundBrush(bgColor);
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
    else { pen->setChecked(true); }
    table->setCursor(QCursor(QPixmap(":/cursor/icons/pen.ico")));
}

void MainWindow::onDashTriggered(bool checked) {
    if (checked) { controller->setDrawController(DashController::getInstance()); }
    else { dash->setChecked(true); }
    table->setCursor(Qt::CrossCursor);
}

void MainWindow::onEraserTriggered(bool checked) {
    if (checked) { controller->setDrawController(EraserController::getInstance()); }
    else { eraser->setChecked(true); }
    table->setCursor(QCursor(QPixmap(":/cursor/icons/eraser.ico")));
}

void MainWindow::onEllipseTriggered(bool checked) {
    if (checked) { controller->setDrawController(EllipseController::getInstance()); }
    else { ellipse->setChecked(true); }
    table->setCursor(Qt::CrossCursor);
}

void MainWindow::onRectangleTriggered(bool checked) {
    if (checked) { controller->setDrawController(RectangleController::getInstance()); }
    else { rectangle->setChecked(true);}
    table->setCursor(Qt::CrossCursor);
}

void MainWindow::onMenuTriggered() {
    MenuDialog dialog(this);
    dialog.exec();
}

void MainWindow::onSaveTriggered() {
    QString fileName = FileDialog::getSaveFileName();
    // creation du conteneur
    QPixmap pixmap(table->width(), table->height());
    // creation du painter allant servir à effectuer notre rendu
    QPainter painter(&pixmap);
    // selection qualite
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    // generation du rendu
    table->render(&painter);
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
        int ret = msgBox.exec();

        switch (ret) {
          case QMessageBox::Save:
              onSaveTriggered();
              table->scene()->clear();
              controller->resetUndoHistory();
          case QMessageBox::Discard:
              table->scene()->clear();
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
        table->scene()->clear();
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
        table->render(&painter);
        painter.end();
    }
}

void MainWindow::onUndoTriggered() {
    controller->undo();
}

void MainWindow::onRedoTriggered() {
    controller->redo();
}

void MainWindow::onThicknessChanged() {
    const QList<QAction*>& actions = thickness->menu()->actions();
    foreach (QAction* a, actions) {
        a->setChecked(false);
    }
    QAction* action = qobject_cast<QAction*>(sender());
    action->setChecked(true);
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
              // should never be reached
              break;
        }

    }else{
        QApplication::quit();
    }
}

void MainWindow::openFile() {
    QString fileName = FileDialog::getOpenFileName();
    QPixmap img(fileName);
    table->scene()->addPixmap(img);
}

void MainWindow::onColorTriggered(){
    QPen* pen = controller->getPen();
    QColorDialog* colorDialog = new QColorDialog(this);
    QColor selectedColor = colorDialog->getColor(pen->color(),this);
    pen->setColor(selectedColor);
}
