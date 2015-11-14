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
    if (event->buttons() == Qt::LeftButton) {
        double rad = 1;
        QPointF point = event->pos();
        //scene->addEllipse(point.x()-rad, point.y()-rad, rad*2.0, rad*2.0, QPen(), QBrush(Qt::SolidPattern));

        path->lineTo(point);
        pathItem->setPath(*path);
    }
}

void PenController::mousePressEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();

    path = new QPainterPath(event->pos());
    pathItem = scene->addPath(*path);
    //Q_UNUSED(scene)
}

void PenController::mouseReleaseEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)
}
