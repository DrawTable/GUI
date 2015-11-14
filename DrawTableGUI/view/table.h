#ifndef TABLE_H
#define TABLE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "../controller/generalcontroller.h"

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
    GeneralController* controller;
};

#endif // TABLE_H
