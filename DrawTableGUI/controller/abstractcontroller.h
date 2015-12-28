#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPen>

/**
 * @brief The AbstractController class
 *
 * Classe abstraite contenant les fonction que doivent implémenter les controleurs
 * pour les dif
 * férents outils de l'application de dessin
 */
class AbstractController {

public:

    /**
     * @brief mouseDoubleClickEvent
     * @param scene : vue à modifier
     * @param event : état de la souris au moment de l'appel
     *
     * Méthode à appeler en cas de double clic
     *
     */
    virtual void mouseDoubleClickEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;

    /**
     * @brief mouseMoveEvent
     * @param scene : vue à modifier
     * @param event : état de la souris au moment de l'appel
     *
     * Méthode à appeler en cas de mouvement de la souris
     *
     */
    virtual void mouseMoveEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;

    /**
     * @brief mousePressEvent
     * @param scene : vue à modifier
     * @param event : état de la souris au moment de l'appel
     * @param pen   : définition du QPen qui détermine la largeur et la couleur du trait
     *
     * Méthode à appeler en cas de pression du bouton gauche de la souris
     *
     */
    virtual void mousePressEvent(QGraphicsScene* scene, QMouseEvent* event, QPen* pen) = 0;

    /**
     * @brief mouseReleaseEvent
     * @param scene : vue à modifier
     * @param event : état de la souris au moment de l'appel
     *
     * @return QGraphicsItem* : Objet final ajouté à la vue à la fin de la manip
     *
     * Méthode à appeler en cas de relachement du bouton gauche de la souris, c'est à
     * ce moment que l'objet courrant est définitivement ajouté à la vue
     *
     */
    virtual QGraphicsItem* mouseReleaseEvent(QGraphicsScene* scene, QMouseEvent* event) = 0;
};

#endif // ABSTRACTCONTROLLER_H

