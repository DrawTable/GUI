#include "ellipsecontroller.h"

EllipseController* EllipseController::instance = nullptr;

EllipseController::EllipseController() : AbstractToolController(), origin(), item(nullptr)
{
}

EllipseController::~EllipseController()
{
}

EllipseController* EllipseController::getInstance()
{
    if (!instance)
    {
        instance = new EllipseController;
    }
    return instance;
}

void EllipseController::mouseDoubleClickEvent(QGraphicsScene *scene, QMouseEvent *event)
{
    Q_UNUSED(scene)
    Q_UNUSED(event)
}

void EllipseController::mouseMoveEvent(QGraphicsScene *scene, QMouseEvent *event)
{
    Q_UNUSED(scene)
    if (item)
    {
        int x = origin.x() > event->x() ? event->x() : origin.x();
        int y = origin.y() > event->y() ? event->y() : origin.y();
        int w = abs(origin.x() - event->x());
        int h = abs(origin.y() - event->y());
        item->setRect(x, y, w, h);
    }

}

void EllipseController::mousePressEvent(QGraphicsScene *scene, QMouseEvent *event, QPen* pen)
{
    origin.setX(event->x());
    origin.setY(event->y());
    item = scene->addEllipse(event->x(), event->y(), 0, 0,*pen);
}

QGraphicsItem* EllipseController::mouseReleaseEvent(QGraphicsScene *scene, QMouseEvent *event)
{
    Q_UNUSED(scene)
    Q_UNUSED(event)
    return item;
}
