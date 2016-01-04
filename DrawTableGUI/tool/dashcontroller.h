#ifndef DASHCONTROLLER_H
#define DASHCONTROLLER_H

#include "abstracttoolcontroller.h"
#include <QGraphicsLineItem>

/**
 * @brief Controller to draw a straight line.
 *
 * This controller inherits from AbstractToolController.
 *
 * Plese refer the the AbstractToolController documentation to see functions' usage.
 */
class DashController : public AbstractToolController
{

public:
    virtual ~DashController();
    static DashController* getInstance();
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event, QPen* pen);
    virtual QGraphicsItem* mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event);

private:
    static DashController* instance;
    QGraphicsLineItem* item;
    DashController();
};

#endif // DASHCONTROLLER_H
