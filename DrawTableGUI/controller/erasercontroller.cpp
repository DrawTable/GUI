#include "erasercontroller.h"
#include <QDebug>

EraserController* EraserController::instance = nullptr;

EraserController::EraserController() : AbstractController() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
}

EraserController::~EraserController() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
}

EraserController* EraserController::getInstance() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (!instance) { instance = new EraserController; }
    return instance;
}

void EraserController::mouseDoubleClickEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)
}

void EraserController::mouseMoveEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)
    QPointF point = event->pos();
    path->lineTo(point);
    pathItem->setPath(*path);
}

void EraserController::mousePressEvent(QGraphicsScene *scene, QMouseEvent *event, QPen* pen) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(pen);
    QPen p(Qt::white);
    p.setWidth(10);
    path = new QPainterPath(event->pos());
    pathItem = scene->addPath(*path, p);
}

QGraphicsItem* EraserController::mouseReleaseEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)
    return nullptr;
}
