#ifndef DRAWINGCONTROLLER_H
#define DRAWINGCONTROLLER_H

#include "../tool/abstracttoolcontroller.h"
#include "drawing.h"
#include <QGraphicsItem>

class Drawing;

/**
 * @brief The DrawingController class
 *
 * Contrôleur général de l'application de dessin gérant quel outil utiliser avec quelle brosse
 * ainsi que la pile des actions effectuées pour les fonction undo() et redo().
 *
 */
class DrawingController {

public:

    /**
     * @brief DrawingController
     * @param drawing : dessin associée au contrôleur
     */
    DrawingController(Drawing* drawing);
    ~DrawingController();

    void setDrawController(AbstractToolController* drawController);
    AbstractToolController* getDrawController();
    void setPen(QPen* pen);
    QPen* getPen();
    void setBrush(QBrush* brush);
    QBrush* getBrush();

    void enable();
    void disable();

    bool canUndo();
    bool canRedo();
    bool toSave();
    void setToSave(bool toSave);
    void resetUndoHistory();

    void undo();
    void redo();
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:

    /**
     * @brief drawController
     *
     * Instsance du contrôleur de dessin utilisé actuellement
     *
     */
    AbstractToolController* drawController;

    /**
     * @brief drawing
     * Dessin sur laquel le contrôleur travaille
     */
    Drawing* drawing;

    /**
     * @brief pen
     * Qpen à utiliser pour dessiner, gère l'épaisseur et la couleur du trait
     */
    QPen* pen;
    QBrush* brush;
    /**
     * @brief lastActions
     * pile des actions effectuées précédemment
     */
    QList<QGraphicsItem*> lastActions;

    /**
     * @brief nextActions
     * pile des actions annulée après un retour en arrière
     */
    QList<QGraphicsItem*> nextActions;

    /**
     * @brief modifToSave
     * Vaut true si une modification sur le dessin a été effectuée depuis
     * la dernière sauvegarde
     */
    bool modifToSave;

    bool isEnable = false;
};

#endif // DRAWINGCONTROLLER_H
