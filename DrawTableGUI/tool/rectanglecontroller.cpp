#include "rectanglecontroller.h"

RectangleController* RectangleController::instance = nullptr;

RectangleController::RectangleController() : AbstractToolController(), origin(), item(nullptr) {
}

RectangleController::~RectangleController() {
}

RectangleController* RectangleController::getInstance() {
    if (!instance) { instance = new RectangleController; }
    return instance;
}

void RectangleController::mouseDoubleClickEvent(QGraphicsScene *scene, QMouseEvent *event) {
    Q_UNUSED(scene)
    Q_UNUSED(event)
}

void RectangleController::mouseMoveEvent(QGraphicsScene *scene, QMouseEvent *event) {
    Q_UNUSED(scene)
    if (item) {
        int x = origin.x() > event->x() ? event->x() : origin.x();
        int y = origin.y() > event->y() ? event->y() : origin.y();
        int w = abs(origin.x() - event->x());
        int h = abs(origin.y() - event->y());
        item->setRect(x, y, w, h);
    }
}

void RectangleController::mousePressEvent(QGraphicsScene *scene, QMouseEvent *event, QPen* pen) {
    origin.setX(event->x());
    origin.setY(event->y());
    item = scene->addRect(event->x(), event->y(), 0, 0, *pen);
}

QGraphicsItem* RectangleController::mouseReleaseEvent(QGraphicsScene *scene, QMouseEvent *event) {
    Q_UNUSED(scene)
    Q_UNUSED(event)
    return item;
}
