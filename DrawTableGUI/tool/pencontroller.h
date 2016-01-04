#ifndef PENCONTROLLER_H
#define PENCONTROLLER_H

#include "abstracttoolcontroller.h"
#include <QGraphicsPathItem>
#include <QPainterPath>

/**
 * @brief Controller to draw a free line with the pen.
 *
 * This controller inherits from AbstractToolController.
 *
 * Plese refer the the AbstractToolController documentation to see functions' usage.
 */
class PenController : public AbstractToolController {

public:
    virtual ~PenController();
    static PenController* getInstance();
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event, QPen* pen);
    virtual QGraphicsItem* mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event);

private:
    QPainterPath* path;
    QGraphicsPathItem* pathItem;
    static PenController* instance;
    PenController();
};

#endif // PENCONTROLLER_H
