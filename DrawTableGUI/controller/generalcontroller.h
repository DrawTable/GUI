#ifndef GENERALCONTROLLER_H
#define GENERALCONTROLLER_H

#include "abstractcontroller.h"
#include "../view/table.h"
#include <QGraphicsItem>

class Table;

/**
 * @brief The GeneralController class
 *
 * Contrôleur général de l'application gérant quel outil utiliser avec quel brosse
 * ainsi que la pile des actions effectuées pour les fonction undo() et redo()
 *
 */
class GeneralController {

public:

    /**
     * @brief GeneralController
     * @param view : vue associée au contrôleur
     */
    GeneralController(Table* view);
    ~GeneralController();

    void setDrawController(AbstractController* drawController);
    AbstractController* getDrawController();
    void setPen(QPen* pen);
    QPen* getPen();
    void setBrush(QBrush* brush);
    QBrush* getBrush();
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
     * Instsance du contrôleur de dessin utilisé actuelement
     *
     */
    AbstractController* drawController;

    /**
     * @brief view
     * Vue sur laquel le contrôleur travail
     */
    Table* view;

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

};

#endif // GENERALCONTROLLER_H
