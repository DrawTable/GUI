#include "mainwindow.h"
#include "../controller/pencontroller.h"
#include "../controller/dashcontroller.h"
#include "../controller/rectanglecontroller.h"
#include "../controller/ellipsecontroller.h"
#include "../controller/generalcontroller.h"
#include "../controller/erasercontroller.h"
#include <QApplication>
#include <QFileDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPixmap>
#include <QMenuBar>
#include <QColorDialog>

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent) {
    // Creation des actions du menu principale
    newImg = new QAction(tr("&New"), this);
    newImg->setShortcut(QKeySequence::New);
    open = new QAction(tr("&Open"), this);
    open->setShortcut(QKeySequence::Open);
    save = new QAction(tr("&Save"), this);
    save->setShortcut(QKeySequence::Save);
    print = new QAction(tr("&Print"), this);
    print->setShortcut(QKeySequence::Print);
    quit = new QAction(tr("&Quit"), this);
    quit->setShortcut(QKeySequence::Quit);

    // Creation du menu et ajout des actions liees a ce dernier
    menu = menuBar()->addMenu("&File");
    menu->addAction(newImg);
    menu->addAction(open);
    menu->addAction(save);
    menu->addAction(print);
    menu->addAction(quit);

    connect(newImg, SIGNAL(triggered()), this, SLOT(onNewTriggered()));
    connect(open, SIGNAL(triggered()), this, SLOT(onOpenTriggered()));
    connect(save, SIGNAL(triggered()), this, SLOT(onSaveTriggered()));
    connect(print, SIGNAL(triggered()), this, SLOT(onPrintTriggered()));
    connect(quit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

    // Creation des actions du menu d'edition
    undo = new QAction(tr("&Undo"), this);
    undo->setShortcut(QKeySequence::Undo);
    redo = new QAction(tr("&redo"), this);
    redo->setShortcut(QKeySequence::Redo);

    // Creation du menu edit et ajout des actions liees a ce dernier
    edit = menuBar()->addMenu("&Edit");
    edit->addAction(undo);
    edit->addAction(redo);

    connect(undo, SIGNAL(triggered()), this, SLOT(onUndoTriggered()));
    connect(redo, SIGNAL(triggered()), this, SLOT(onRedoTriggered()));

    // Creation de la vue
    QBrush bgColor(Qt::white);
    table = new Table(this);
    table->setBackgroundBrush(bgColor);
    table->scene()->setSceneRect(0, 0, maximumWidth(), maximumHeight());

    // Creation des actions de la toolbar
    cursor = new QAction(this);
    cursor->setCheckable(true);
    cursor->setIcon(QIcon(":/svg/icons/cursor.svg"));
    connect(cursor, SIGNAL(triggered(bool)), this, SLOT(onCursorTriggered(bool)));

    pen = new QAction(this);
    pen->setCheckable(true);
    pen->setChecked(true);
    pen->setIcon(QIcon(":/svg/icons/pen.svg"));
    connect(pen, SIGNAL(triggered(bool)), this, SLOT(onPenTriggered(bool)));

    dash = new QAction(this);
    dash->setCheckable(true);
    dash->setIcon(QIcon(":/svg/icons/dash.svg"));
    connect(dash, SIGNAL(triggered(bool)), this, SLOT(onDashTriggered(bool)));

    eraser = new QAction(this);
    eraser->setCheckable(true);
    eraser->setIcon(QIcon(":/svg/icons/eraser.svg"));
    connect(eraser, SIGNAL(triggered(bool)), this, SLOT(onEraserTriggered(bool)));

    ellipse = new QAction(this);
    ellipse->setCheckable(true);
    ellipse->setIcon(QIcon(":/svg/icons/ellipse.svg"));
    connect(ellipse, SIGNAL(triggered(bool)), this, SLOT(onEllipseTriggered(bool)));

    rectangle = new QAction(this);
    rectangle->setCheckable(true);
    rectangle->setIcon(QIcon(":/svg/icons/rectangle.svg"));
    connect(rectangle, SIGNAL(triggered(bool)), this, SLOT(onRectangleTriggered(bool)));

    color = new QAction(this);
    color->setIcon(QIcon(":/svg/icons/color.svg"));
    connect(color, SIGNAL(triggered()), this, SLOT(onColorTriggered()));

    QList<QIcon> icons;
    for (int i = 0; i < 6; ++i) {
        icons << QIcon(QString(":/svg/icons/thickness%1.svg").arg(i + 1));
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
    thicknessMenu->actions().at(0)->setChecked(true);

    thickness = new QToolButton(this);
    thickness->setIcon(QIcon(":/svg/icons/thickness.svg"));
    thickness->setPopupMode(QToolButton::MenuButtonPopup);
    thickness->setMenu(thicknessMenu);

    // Creation de la toolbar et integration des actions
    toolBar = new QToolBar(tr("Tools"));
    toolBar->addAction(cursor);
    toolBar->addSeparator();
    toolBar->addAction(pen);
    toolBar->addAction(eraser);
    toolBar->addAction(dash);
    toolBar->addWidget(thickness);
    toolBar->addSeparator();
    toolBar->addAction(ellipse);
    toolBar->addAction(rectangle);
    toolBar->addSeparator();
    toolBar->addAction(color);
    connect(toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(updateToolBarActions(QAction*)));
    addToolBar(Qt::RightToolBarArea, toolBar);

    // Creation du controleur principale
    controller = new GeneralController(table);

    // Integration de la vue a la GUI
    setCentralWidget(table);
    showFullScreen();

    startTrackingManager();
}

MainWindow::~MainWindow() {
}

// Crée et démarre un thread qui gère le tracking du stylet
void MainWindow::startTrackingManager() {
    QThread* thread = new QThread;
    TrackingManager* worker = new TrackingManager();
    worker->moveToThread(thread);

    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void MainWindow::updateToolBarActions(QAction* action) {
    if (action != color){
        const QList<QAction*>& actions = toolBar->actions();
        foreach(QAction* a, actions) {
            if (a != action) { a->setChecked(false); }
        }
    }
}

void MainWindow::onCursorTriggered(bool checked) {
    if (checked) {}
    else { cursor->setChecked(true); }
}

void MainWindow::onPenTriggered(bool checked) {
    if (checked) { controller->setDrawController(PenController::getInstance()); }
    else { pen->setChecked(true); }
}

void MainWindow::onDashTriggered(bool checked) {
    if (checked) { controller->setDrawController(DashController::getInstance()); }
    else { dash->setChecked(true); }
}

void MainWindow::onEraserTriggered(bool checked) {
    if (checked) { controller->setDrawController(EraserController::getInstance()); }
    else { eraser->setChecked(true); }
}

void MainWindow::onEllipseTriggered(bool checked) {
    if (checked) { controller->setDrawController(EllipseController::getInstance()); }
    else { ellipse->setChecked(true); }
}

void MainWindow::onRectangleTriggered(bool checked) {
    if (checked) { controller->setDrawController(RectangleController::getInstance()); }
    else { rectangle->setChecked(true);}
}

void MainWindow::onSaveTriggered() {
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "C:/", tr("Images (*.png *.bmp *.jpg)"));

    // creation du conteneur
    QPixmap pixmap(table->width(), table->height());
    // creation du painter allant servir à effectuer notre rendu
    QPainter painter(&pixmap);
    // selection qualite
    painter.setRenderHint(QPainter::Antialiasing);
    // generation du rendu
    table->render(&painter);
    // enregistrement
    pixmap.save(fileName);
    painter.end();
}

void MainWindow::onOpenTriggered() {
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "C:/",
                                                    tr("Images (*.png *.bmp *.jpg)"));
    QPixmap img(fileName);
    table->scene()->addPixmap(img);
}

void MainWindow::onNewTriggered() {
    table->scene()->clear();
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
    foreach(QAction* a, actions) {
        a->setChecked(false);
    }
    QAction* action = qobject_cast<QAction*>(sender());
    action->setChecked(true);
    QPen* pen = controller->getPen();
    pen->setWidth(qvariant_cast<int>(action->data()));
}

void MainWindow::onColorTriggered(){
    QPen* pen = controller->getPen();
    QColorDialog* colorDialog = new QColorDialog(this);
    QColor selectedColor = colorDialog->getColor(pen->color(),this);
    pen->setColor(selectedColor);
}
