#ifndef ABSTRACTTOOLCONTROLLER_H
#define ABSTRACTTOOLCONTROLLER_H

#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPen>

/**
 * @brief The AbstractToolController class
 *
 * Abstract class containing every functions that controllers must implements.
 * Those function will then be called by the tools.
 */
class AbstractToolController {

public:

    /**
     * @brief Method called when the user double clicks.
     * @param scene : view to modify
     * @param event : state of the mouse
     */
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;

    /**
     * @brief Method called when the user mouves the mouse.
     * @param scene : view to modify
     * @param event : state of the mouse
     */
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;

    /**
     * @brief Method called when the user presses a mouse button.
     * @param scene : view to modify
     * @param event : state of the mouse
     * @param pen   : definition of the QPen that determinates the thickness of the line.
     */
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event, QPen* pen) = 0;

    /**
     * @brief Method called when the user releases a mouse button.
     * @param scene : view to modify
     * @param event : state of the mouse
     *
     * @return QGraphicsItem* : Final objet added to the scene at the end of the process.
     */
    virtual QGraphicsItem* mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
};

#endif // ABSTRACTTOOLCONTROLLER_H
