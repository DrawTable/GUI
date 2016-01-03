#ifndef RECTANGLECONTROLLER_H
#define RECTANGLECONTROLLER_H

#include "abstracttoolcontroller.h"
#include <QPoint>
#include <QGraphicsRectItem>


/**
 * @brief The RectangleController class
 *
 * Contrôleur gérant le dessin de rectangles, hérite de AbstractToolController
 *
 */
class RectangleController : public AbstractToolController {

public:
    virtual ~RectangleController();
    static RectangleController* getInstance();
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event);
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event, QPen* pen);
    virtual QGraphicsItem* mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event);

private:
    static RectangleController* instance;
    QPoint origin;
    QGraphicsRectItem* item;
    RectangleController();
};

#endif // RECTANGLECONTROLLER_H
