#ifndef DRAWING_H
#define DRAWING_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "drawingcontroller.h"

class DrawingController;

/**
 * @brief The Drawing class
 *
 * Classe modèle de dessin, elle possède une redéfinition des
 * méthode appelée lors des QMouseEvent qui en délégue la gestion
 * au GeneralController
 *  *
 */
class Drawing : public QGraphicsView {
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
