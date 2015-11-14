#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPen>

class AbstractController {

public:
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event, QPen* pen) = 0;
    virtual QGraphicsItem* mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
};

#endif // ABSTRACTCONTROLLER_H

