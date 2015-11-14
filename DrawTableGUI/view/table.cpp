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

GeneralController *Table::getController() const {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    return controller;
}

void Table::setController(GeneralController *controller) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    this->controller = controller;
}

void Table::mouseDoubleClickEvent(QMouseEvent* event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    controller->mouseDoubleClickEvent(event);
}

void Table::mouseMoveEvent(QMouseEvent* event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    controller->mouseMoveEvent(event);
}

void Table::mousePressEvent(QMouseEvent* event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    controller->mousePressEvent(event);
}

void Table::mouseReleaseEvent(QMouseEvent* event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    controller->mouseDoubleClickEvent(event);
}
