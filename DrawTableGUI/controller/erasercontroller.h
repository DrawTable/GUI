#ifndef ERASERCONTROLLER_H
#define ERASERCONTROLLER_H

#include "abstractcontroller.h"
#include <QGraphicsPathItem>
#include <QPainterPath>

class EraserController : public AbstractController {

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
