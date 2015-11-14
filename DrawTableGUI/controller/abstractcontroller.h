#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QMouseEvent>
#include <QGraphicsScene>

class AbstractController {

public:
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
    virtual void mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
};

#endif // ABSTRACTCONTROLLER_H

