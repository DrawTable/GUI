#ifndef DASHCONTROLLER_H
#define DASHCONTROLLER_H

#include "abstractcontroller.h"
#include <QGraphicsLineItem>

class DashController : public AbstractController {

public:
    virtual ~DashController();
    static DashController* getInstance();
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event);

private:
    static DashController* instance;
    QGraphicsLineItem* item;
    DashController();
};

#endif // DASHCONTROLLER_H
