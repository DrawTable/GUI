#include "ellipsecontroller.h"

#include <QDebug>

EllipseController* EllipseController::instance = nullptr;

EllipseController::EllipseController() : AbstractController(), origin(), item(nullptr) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
}

EllipseController::~EllipseController() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
}

EllipseController* EllipseController::getInstance() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (!instance) { instance = new EllipseController; }
    return instance;
}

void EllipseController::mouseDoubleClickEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)
}

void EllipseController::mouseMoveEvent(QGraphicsScene *scene, QMouseEvent *event) {
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

void EllipseController::mousePressEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    origin.setX(event->x());
    origin.setY(event->y());
    item = scene->addEllipse(event->x(), event->y(), 0, 0,QPen(Qt::red), QBrush(Qt::yellow));
}

QGraphicsItem* EllipseController::mouseReleaseEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)

    origin.setX(0);
    origin.setY(0);
    return item;
}
