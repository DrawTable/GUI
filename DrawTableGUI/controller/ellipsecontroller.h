#ifndef CIRCLECONTROLLER_H
#define CIRCLECONTROLLER_H

#include "abstractcontroller.h"
#include <QPoint>
#include <QGraphicsEllipseItem>

class EllipseController : public AbstractController {

public:
    virtual ~EllipseController();
    static EllipseController* getInstance();
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event);

private:
    static EllipseController* instance;
    QPoint origin;
    QGraphicsEllipseItem* item;
    EllipseController();
};

#endif // CIRCLECONTROLLER_H
