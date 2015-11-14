#include "mainwindow.h"
#include "../controller/pencontroller.h"
#include "../controller/dashcontroller.h"
#include "../controller/rectanglecontroller.h"
#include "../controller/ellipsecontroller.h"
#include <QDebug>

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";

    table = new Table(this);
    table->setController(PenController::getInstance());
    table->scene()->setSceneRect(0, 0, maximumWidth(), maximumHeight());

    cursor = new QAction(this);
    cursor->setCheckable(true);
    cursor->setIcon(QIcon(":/svg/cursor.svg"));
    connect(cursor, SIGNAL(triggered(bool)), this, SLOT(onCursorTriggered(bool)));

    pen = new QAction(this);
    pen->setCheckable(true);
    pen->setChecked(true);
    pen->setIcon(QIcon(":/svg/pen.svg"));
    connect(pen, SIGNAL(triggered(bool)), this, SLOT(onPenTriggered(bool)));

    dash = new QAction(this);
    dash->setCheckable(true);
    dash->setIcon(QIcon(":/svg/dash.svg"));
    connect(dash, SIGNAL(triggered(bool)), this, SLOT(onDashTriggered(bool)));

    eraser = new QAction(this);
    eraser->setCheckable(true);
    eraser->setIcon(QIcon(":/svg/eraser.svg"));
    connect(eraser, SIGNAL(triggered(bool)), this, SLOT(onEraserTriggered(bool)));

    ellipse = new QAction(this);
    ellipse->setCheckable(true);
    ellipse->setIcon(QIcon(":/svg/ellipse.svg"));
    connect(ellipse, SIGNAL(triggered(bool)), this, SLOT(onEllipseTriggered(bool)));

    rectangle = new QAction(this);
    rectangle->setCheckable(true);
    rectangle->setIcon(QIcon(":/svg/rectangle.svg"));
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

    setCentralWidget(table);

    showFullScreen();
}

MainWindow::~MainWindow() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
}

void MainWindow::updateToolBarActions(QAction* action) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    const QList<QAction*>& actions = toolBar->actions();
    foreach(QAction* a, actions) {
        if (a != action) { a->setChecked(false); }
    }
}

void MainWindow::onCursorTriggered(bool checked) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (checked) {}
    else {cursor->setChecked(true);}
}

void MainWindow::onPenTriggered(bool checked) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (checked) { table->setController(PenController::getInstance()); }
    else {pen->setChecked(true);}
}

void MainWindow::onDashTriggered(bool checked) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (checked) { table->setController(DashController::getInstance()); }
    else {dash->setChecked(true);}
}

void MainWindow::onEraserTriggered(bool checked) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (checked) {}
    else {eraser->setChecked(true);}
}

void MainWindow::onEllipseTriggered(bool checked) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (checked) { table->setController(EllipseController::getInstance()); }
    else {ellipse->setChecked(true);}
}

void MainWindow::onRectangleTriggered(bool checked) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (checked) { table->setController(RectangleController::getInstance()); }
    else {rectangle->setChecked(true);}
}