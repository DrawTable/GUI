#ifndef DRAWING_H
#define DRAWING_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "drawingcontroller.h"

class DrawingController;

/**
 * @brief Draw zone
 *
 * This view inherits from a QGraphicsView and redefines mouse events handlers.
 * Those handlers will then delegate everything to the DrawingController.
 */
class Drawing : public QGraphicsView
{
    Q_OBJECT

public:
    Drawing(QWidget* parent = 0);

    DrawingController* getController() const;
    void setController(DrawingController* controller);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    /**
     * @brief controller
     * Contrôleur associé au dessin
     */
    DrawingController* controller;
};

#endif // DRAWING_H
