#include "table.h"
#include <QDebug>

Table::Table(QWidget*parent) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";

    QGraphicsScene* scene = new QGraphicsScene(parent);
    setScene(scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);
}

AbstractController *Table::getController() const {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    return controller;
}

void Table::setController(AbstractController *controller) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    this->controller = controller;
}

void Table::mouseDoubleClickEvent(QMouseEvent* event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    controller->mouseDoubleClickEvent(scene(), event);
}

void Table::mouseMoveEvent(QMouseEvent* event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    controller->mouseMoveEvent(scene(), event);
}

void Table::mousePressEvent(QMouseEvent* event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    controller->mousePressEvent(scene(), event);
}

void Table::mouseReleaseEvent(QMouseEvent* event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    controller->mouseDoubleClickEvent(scene(), event);
}
