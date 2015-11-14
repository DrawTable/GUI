#ifndef ICONTROLLER
#define ICONTROLLER

#include <QMouseEvent>
#include <QGraphicsScene>

class AbstractController {

public:
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
    virtual void mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
};

#endif // ICONTROLLER

