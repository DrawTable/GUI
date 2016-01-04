#ifndef ERASERCONTROLLER_H
#define ERASERCONTROLLER_H

#include "abstracttoolcontroller.h"
#include <QGraphicsPathItem>
#include <QPainterPath>

/**
 * @brief Controller to erase any line.
 *
 * This controller inherits from AbstractToolController.
 *
 * Plese refer the the AbstractToolController documentation to see functions' usage.
 */
class EraserController : public AbstractToolController
{

public:
    virtual ~EraserController();
    static EraserController* getInstance();
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event, QPen* pen);
    virtual QGraphicsItem* mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event);

private:
    QPainterPath* path;
    QGraphicsPathItem* pathItem;
    static EraserController* instance;
    EraserController();
};

#endif // ERASERCONTROLLER_H
