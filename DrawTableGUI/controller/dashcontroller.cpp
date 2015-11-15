#include "dashcontroller.h"

DashController* DashController::instance = nullptr;

DashController::DashController() : AbstractController(), item(nullptr) {
}

DashController::~DashController() {
}

DashController* DashController::getInstance() {
    if (!instance) { instance = new DashController; }
    return instance;
}

void DashController::mouseDoubleClickEvent(QGraphicsScene *scene, QMouseEvent *event) {
    Q_UNUSED(scene)
    Q_UNUSED(event)
}

void DashController::mouseMoveEvent(QGraphicsScene *scene, QMouseEvent *event) {
    Q_UNUSED(scene)
    if (item) { item->setLine(item->line().x1(), item->line().y1(), event->x(), event->y()); }
}

void DashController::mousePressEvent(QGraphicsScene *scene, QMouseEvent *event, QPen* pen) {
    item = scene->addLine(event->x(), event->y(), event->x(), event->y(), *pen);
}

QGraphicsItem* DashController::mouseReleaseEvent(QGraphicsScene *scene, QMouseEvent *event) {
    Q_UNUSED(scene)
    Q_UNUSED(event)
    return item;
}
