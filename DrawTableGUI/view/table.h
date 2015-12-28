#ifndef TABLE_H
#define TABLE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "../controller/generalcontroller.h"

class GeneralController;

/**
 * @brief The Table class
 *
 * Vue utilisée pour le dessin, elle possède une redéfinition des
 * méthode appelée lors des QMouseEvent qui en délégue la gestion
 * au GeneralController
 *  *
 */
class Table : public QGraphicsView {
    Q_OBJECT

public:
    Table(QWidget* parent = 0);

    GeneralController* getController() const;
    void setController(GeneralController* controller);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    /**
     * @brief controller
     * Contrôleur associé à la vue
     */
    GeneralController* controller;
};

#endif // TABLE_H
