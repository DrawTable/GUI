#include "rectanglecontroller.h"
#include <QDebug>

RectangleController* RectangleController::instance = nullptr;

RectangleController::RectangleController() : AbstractController(), origin(), item(nullptr) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
}

RectangleController::~RectangleController() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
}

RectangleController* RectangleController::getInstance() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (!instance) { instance = new RectangleController; }
    return instance;
}

void RectangleController::mouseDoubleClickEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)
}

void RectangleController::mouseMoveEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)
    if (event->buttons() == Qt::LeftButton) {
        if (item) {
            int x = origin.x() > event->x() ? event->x() : origin.x();
            int y = origin.y() > event->y() ? event->y() : origin.y();
            int w = abs(origin.x() - event->x());
            int h = abs(origin.y() - event->y());
            item->setRect(x, y, w, h);
        }
    }
}

void RectangleController::mousePressEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    origin.setX(event->x());
    origin.setY(event->y());
    item = scene->addRect(event->x(), event->y(), 0, 0);
}

QGraphicsItem* RectangleController::mouseReleaseEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)

    origin.setX(0);
    origin.setY(0);
    return item;
}
