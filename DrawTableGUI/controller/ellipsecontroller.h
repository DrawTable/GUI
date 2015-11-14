#ifndef ELLIPSECONTROLLER_H
#define ELLIPSECONTROLLER_H

#include "abstractcontroller.h"
#include <QPoint>
#include <QGraphicsEllipseItem>

class EllipseController : public AbstractController {

public:
    virtual ~EllipseController();
    static EllipseController* getInstance();
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event, QPen* pen);
    virtual QGraphicsItem* mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event);

private:
    static EllipseController* instance;
    QPoint origin;
    QGraphicsEllipseItem* item;
    EllipseController();
};

#endif // ELLIPSECONTROLLER_H
