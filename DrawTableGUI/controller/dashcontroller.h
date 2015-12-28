#ifndef DASHCONTROLLER_H
#define DASHCONTROLLER_H

#include "abstractcontroller.h"
#include <QGraphicsLineItem>

/**
 * @brief The DashController class
 *
 * Contrôleur gérant le dessin de trait droit, hérite de AbstractController
 *
 */
class DashController : public AbstractController {

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
