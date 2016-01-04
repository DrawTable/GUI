#include "erasercontroller.h"

EraserController* EraserController::instance = nullptr;

EraserController::EraserController() : AbstractToolController()
{
}

EraserController::~EraserController()
{
}

EraserController* EraserController::getInstance()
{
    if (!instance)
    {
        instance = new EraserController;
    }
    return instance;
}

void EraserController::mouseDoubleClickEvent(QGraphicsScene *scene, QMouseEvent *event)
{
    Q_UNUSED(scene)
    Q_UNUSED(event)
}

void EraserController::mouseMoveEvent(QGraphicsScene *scene, QMouseEvent *event)
{
    Q_UNUSED(scene)
    QPointF point = event->pos();
    path->lineTo(point);
    pathItem->setPath(*path);
}

void EraserController::mousePressEvent(QGraphicsScene *scene, QMouseEvent *event, QPen* pen)
{
    Q_UNUSED(pen);
    path = new QPainterPath(event->pos());
    pathItem = scene->addPath(*path, QPen(QBrush(Qt::black), pen->width()));
}

QGraphicsItem* EraserController::mouseReleaseEvent(QGraphicsScene *scene, QMouseEvent *event)
{
    Q_UNUSED(scene)
    Q_UNUSED(event)
    return nullptr;
}
