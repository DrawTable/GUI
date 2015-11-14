#include "mainwindow.h"
#include "../controller/pencontroller.h"
#include "../controller/dashcontroller.h"
#include "../controller/rectanglecontroller.h"
#include "../controller/ellipsecontroller.h"
#include "../controller/generalcontroller.h"
#include <QDebug>
#include <QApplication>
#include <QFileDialog>

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";

    // Creation des actions du menu
    newImg = new QAction("&New", this);
    open = new QAction("&Open", this);
    save = new QAction("&Save", this);
    quit = new QAction("&Quit", this);
    // Creation du menu et ajout des actions a ce dernier
    menu = menuBar()->addMenu("&File");
    menu->addAction(newImg);
    menu->addAction(open);
    menu->addAction(save);
    menu->addAction(quit);

    connect(newImg, SIGNAL(triggered()), this, SLOT(onNewTriggered()));
    connect(open, SIGNAL(triggered()), this, SLOT(onOpenTriggered()));
    connect(save, SIGNAL(triggered()), this, SLOT(onSaveTriggered()));
    connect(quit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

    QBrush bgColor(Qt::white);
    table = new Table(this);
    table->setBackgroundBrush(bgColor);

    //table->setController(PenController::getInstance());

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

    controller = new GeneralController(table);
    controller->setPen(new QPen(Qt::red));


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
    if (checked) { controller->setDrawController(PenController::getInstance()); }
    else {pen->setChecked(true);}
}

void MainWindow::onDashTriggered(bool checked) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (checked) { controller->setDrawController(DashController::getInstance()); }
    else {dash->setChecked(true);}
}

void MainWindow::onEraserTriggered(bool checked) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (checked) {}
    else {eraser->setChecked(true);}
}

void MainWindow::onEllipseTriggered(bool checked) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (checked) { controller->setDrawController(EllipseController::getInstance()); }
    else {ellipse->setChecked(true);}
}

void MainWindow::onRectangleTriggered(bool checked) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (checked) { controller->setDrawController(RectangleController::getInstance()); }
    else {rectangle->setChecked(true);}
}

void MainWindow::onSaveTriggered(){
    qDebug() << "Function:" << Q_FUNC_INFO << "called";

    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "C:/", tr("Images (*.png)"));

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

void MainWindow::onOpenTriggered(){
    qDebug() << "Function:" << Q_FUNC_INFO << "called";

    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "C:/",
                                                    tr("Images (*.png *.bmp *.jpg)"));
    QPixmap img(fileName);
    table->scene()->addPixmap(img);
}

void MainWindow::onNewTriggered(){
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    table->scene()->clear();
}
