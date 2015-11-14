#include "dashcontroller.h"
#include <QDebug>

DashController* DashController::instance = nullptr;

DashController::DashController() : AbstractController(), item(nullptr) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
}

DashController::~DashController() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
}

DashController* DashController::getInstance() {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    if (!instance) { instance = new DashController; }
    return instance;
}

void DashController::mouseDoubleClickEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)
}

void DashController::mouseMoveEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)

    if (item) { item->setLine(item->line().x1(), item->line().y1(), event->x(), event->y()); }

}

void DashController::mousePressEvent(QGraphicsScene *scene, QMouseEvent *event, QPen* pen) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    item = scene->addLine(event->x(), event->y(), event->x(), event->y(), *pen);
}

QGraphicsItem* DashController::mouseReleaseEvent(QGraphicsScene *scene, QMouseEvent *event) {
    qDebug() << "Function:" << Q_FUNC_INFO << "called";
    qDebug() << "x:" << event->x() << " y: " << event->y();
    Q_UNUSED(scene)
    return item;

}

