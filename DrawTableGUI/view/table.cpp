#include "table.h"

Table::Table(QWidget* parent) {
    QGraphicsScene* scene = new QGraphicsScene(parent);
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMouseTracking(true);
}

GeneralController *Table::getController() const {
    return controller;
}

void Table::setController(GeneralController *controller) {
    this->controller = controller;
}

void Table::mouseDoubleClickEvent(QMouseEvent* event) {
    controller->mouseDoubleClickEvent(event);
}

void Table::mouseMoveEvent(QMouseEvent* event) {
    controller->mouseMoveEvent(event);
}

void Table::mousePressEvent(QMouseEvent* event) {
    controller->mousePressEvent(event);
}

void Table::mouseReleaseEvent(QMouseEvent* event) {
    controller->mouseDoubleClickEvent(event);
}
