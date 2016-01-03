#ifndef ELLIPSECONTROLLER_H
#define ELLIPSECONTROLLER_H

#include "abstracttoolcontroller.h"
#include <QPoint>
#include <QGraphicsEllipseItem>

/**
 * @brief The EllipseController class
 *
 * Contrôleur gérant le dessin d'élipse, hérite de AbstractToolController
 */
class EllipseController : public AbstractToolController {

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
