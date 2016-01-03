#ifndef DRAWINGCONTROLLER_H
#define DRAWINGCONTROLLER_H

#include "../tool/abstracttoolcontroller.h"
#include "drawing.h"
#include <QGraphicsItem>

class Drawing;

/**
 * @brief Controller of the Drawing
 *
 * General controller of the application handling drawing tools and its parameters (color, thickness).
 * It also handles a stack of user's actions to allow him to use the undo and redo functions.
 */
class DrawingController {

public:

    /**
     * @brief Create a DrawingController for a given Drawing.
     * @param drawing View to associate with this controller.
     */
    DrawingController(Drawing* drawing);
    ~DrawingController();

    /**
     * @brief Set a new tool controller to be used.
     * @param drawController An new AbstractToolController.
     */
    void setDrawController(AbstractToolController* drawController);

    /**
     * @brief Get the current tool controller.
     * @return A tool controller.
     */
    AbstractToolController* getDrawController();

    /**
     * @brief Set a QPen for the DrawingController.
     * @param pen A pen.
     */
    void setPen(QPen* pen);

    /**
     * @brief Get the QPen of the DrawingController.
     * @return The pen.
     */
    QPen* getPen();

    /**
     * @brief Set a QBrush for the DrawingController.
     * @param brush A brush.
     */
    void setBrush(QBrush* brush);

    /**
     * @brief Get the QBrush of the DrawingController.
     * @return The brush.
     */
    QBrush* getBrush();

    /**
     * @brief Enable the user to use the draw zone.
     */
    void enable();

    /**
     * @brief Disable the user to use the draw zone.
     */
    void disable();

    /**
     * @brief Check if the user can undo his last action.
     * @return true if the user has some actions to undo.
     */
    bool canUndo();

    /**
     * @brief Check if the user can redo his undone action.
     * @return true if the user has some actions to redo.
     */
    bool canRedo();

    /**
     * @brief Check if the user has a drawing to save.
     * @return true if the user hasn't save its drawing.
     */
    bool toSave();

    /**
     * @brief Define if the user has modify his drawing.
     * @param toSave true if the user has something to save.
     */
    void setToSave(bool toSave);

    /**
     * @brief Reset the undo history. It is mainly used when a user create a new drawing.
     */
    void resetUndoHistory();

    /**
     * @brief Undo the user last action.
     */
    void undo();

    /**
     * @brief Redo the user undone action.
     */
    void redo();

    /**
     * @brief Event triggered when the user double clicks.
     * @param event
     */
    virtual void mouseDoubleClickEvent(QMouseEvent* event);

    /**
     * @brief Event triggered when the user moves the mouse.
     * @param event
     */
    virtual void mouseMoveEvent(QMouseEvent* event);

    /**
     * @brief Event triggered when the user presses a mouse button.
     * @param event
     */
    virtual void mousePressEvent(QMouseEvent* event);

    /**
     * @brief Event triggered when the user releases a mouse button.
     * @param event
     */
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    AbstractToolController* drawController;
    Drawing* drawing;
    QPen* pen;
    QBrush* brush;
    QList<QGraphicsItem*> lastActions;
    QList<QGraphicsItem*> nextActions;
    bool modifToSave = false;
    bool isEnable = false;
};

#endif // DRAWINGCONTROLLER_H
