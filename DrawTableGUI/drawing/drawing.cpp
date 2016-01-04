#include "drawing.h"

Drawing::Drawing(QWidget* parent)
{
    QGraphicsScene* scene = new QGraphicsScene(parent);
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::HighQualityAntialiasing);
    setMouseTracking(true);
}

DrawingController *Drawing::getController() const
{
    return controller;
}

void Drawing::setController(DrawingController *controller)
{
    this->controller = controller;
}

void Drawing::mouseDoubleClickEvent(QMouseEvent* event)
{
    controller->mouseDoubleClickEvent(event);
}

void Drawing::mouseMoveEvent(QMouseEvent* event)
{
    controller->mouseMoveEvent(event);
}

void Drawing::mousePressEvent(QMouseEvent* event)
{
    controller->mousePressEvent(event);
}

void Drawing::mouseReleaseEvent(QMouseEvent* event)
{
    controller->mouseReleaseEvent(event);
}
