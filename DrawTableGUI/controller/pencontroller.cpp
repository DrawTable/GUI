#include "pencontroller.h"

PenController* PenController::instance = nullptr;

PenController::PenController() : AbstractController() {
}

PenController::~PenController() {
}

PenController* PenController::getInstance() {
    if (!instance) { instance = new PenController; }
    return instance;
}

void PenController::mouseDoubleClickEvent(QGraphicsScene *scene, QMouseEvent *event) {
    Q_UNUSED(scene)
    Q_UNUSED(event)
}

void PenController::mouseMoveEvent(QGraphicsScene *scene, QMouseEvent *event) {
    Q_UNUSED(scene)
    QPointF point = event->pos();
    path->lineTo(point);
    pathItem->setPath(*path);
}

void PenController::mousePressEvent(QGraphicsScene *scene, QMouseEvent *event, QPen* pen) {
    path = new QPainterPath(event->pos());
    pathItem = scene->addPath(*path, *pen);
}

QGraphicsItem* PenController::mouseReleaseEvent(QGraphicsScene *scene, QMouseEvent *event) {
    Q_UNUSED(scene)
    Q_UNUSED(event)
    return pathItem;
}
