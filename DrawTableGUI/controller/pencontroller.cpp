#include "pencontroller.h"
#include <QDebug>

PenController* PenController::instance = nullptr;

PenController::PenController() : AbstractController() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
}

PenController::~PenController() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
}

PenController* PenController::getInstance() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (!instance) { instance = new PenController; }
    return instance;
}

void PenController::mouseDoubleClickEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)
}

void PenController::mouseMoveEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();


    QPointF point = event->pos();

    path->lineTo(point);
    pathItem->setPath(*path);

}

void PenController::mousePressEvent(QGraphicsScene *scene, QMouseEvent *event, QPen* pen) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();

    path = new QPainterPath(event->pos());
    pathItem = scene->addPath(*path, *pen);
    //Q_UNUSED(scene)
}

QGraphicsItem* PenController::mouseReleaseEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)
    return pathItem;
}
