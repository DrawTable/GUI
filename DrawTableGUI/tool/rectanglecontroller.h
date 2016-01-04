#ifndef RECTANGLECONTROLLER_H
#define RECTANGLECONTROLLER_H

#include "abstracttoolcontroller.h"
#include <QPoint>
#include <QGraphicsRectItem>

/**
 * @brief Controller to draw a rectangle.
 *
 * This controller inherits from AbstractToolController.
 *
 * Plese refer the the AbstractToolController documentation to see functions' usage.
 */
class RectangleController : public AbstractToolController
{

public:
    virtual ~RectangleController();
    static RectangleController* getInstance();
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event, QPen* pen);
    virtual QGraphicsItem* mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event);

private:
    static RectangleController* instance;
    QPoint origin;
    QGraphicsRectItem* item;
    RectangleController();
};

#endif // RECTANGLECONTROLLER_H
