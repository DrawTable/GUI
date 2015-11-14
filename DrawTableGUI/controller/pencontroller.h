#ifndef PENCONTROLLER_H
#define PENCONTROLLER_H

#include "abstractcontroller.h"
#include <QGraphicsPathItem>
#include <QPainterPath>

class PenController : public AbstractController {

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
