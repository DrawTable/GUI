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

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent) {
    // Creation des actions du menu
    newImg = new QAction(tr("&New"), this);
    open = new QAction(tr("&Open"), this);
    save = new QAction(tr("&Save"), this);
    print = new QAction(tr("&Print"), this);
    quit = new QAction(tr("&Quit"), this);

    // Creation du menu et ajout des actions a ce dernier
    menu = menuBar()->addMenu("&File");
    menu->addAction(newImg);
    menu->addAction(open);
    menu->addAction(save);
    menu->addAction(print);
    menu->addAction(quit);

    undo = new QAction(tr("&Undo"), this);
    redo = new QAction(tr("&redo"), this);

    edit = menuBar()->addMenu("&Edit");
    edit->addAction(undo);
    edit->addAction(redo);

    connect(undo, SIGNAL(triggered()), this, SLOT(onUndoTriggered()));
    connect(redo, SIGNAL(triggered()), this, SLOT(onRedoTriggered()));

    connect(newImg, SIGNAL(triggered()), this, SLOT(onNewTriggered()));
    connect(open, SIGNAL(triggered()), this, SLOT(onOpenTriggered()));
    connect(save, SIGNAL(triggered()), this, SLOT(onSaveTriggered()));
    connect(print, SIGNAL(triggered()), this, SLOT(onPrintTriggered()));
    connect(quit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

    QBrush bgColor(Qt::white);
    table = new Table(this);
    table->setBackgroundBrush(bgColor);
    table->scene()->setSceneRect(0, 0, maximumWidth(), maximumHeight());

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

    toolBar = new QToolBar(tr("Tools"));
    toolBar->addAction(cursor);
    toolBar->addAction(pen);
    toolBar->addAction(eraser);
    toolBar->addAction(dash);
    toolBar->addAction(ellipse);
    toolBar->addAction(rectangle);
    connect(toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(updateToolBarActions(QAction*)));
    addToolBar(Qt::RightToolBarArea, toolBar);

    controller = new GeneralController(table);


    setCentralWidget(table);

    showFullScreen();
}

MainWindow::~MainWindow() {
}

void MainWindow::updateToolBarActions(QAction* action) {
    const QList<QAction*>& actions = toolBar->actions();
    foreach(QAction* a, actions) {
        if (a != action) { a->setChecked(false); }
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

void MainWindow::onUndoTriggered(){
    controller->undo();
}

void MainWindow::onRedoTriggered(){
    controller->redo();
}
