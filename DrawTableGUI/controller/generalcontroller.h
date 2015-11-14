#ifndef GENERALCONTROLLER_H
#define GENERALCONTROLLER_H

#include "abstractcontroller.h"
#include "../view/table.h"
#include <QGraphicsItem>


class GeneralController
{
public:

    GeneralController(Table* view);
    ~GeneralController();

    void setDrawController(AbstractController* drawController);
    AbstractController* getDrawController();
    void setPen(QPen* pen);
    QPen* getPen();
    void setBrush(QBrush* brush);
    QBrush* getBrush();

    void undo();
    void redo();
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:

    AbstractController* drawController;
    Table* view;
    QPen* pen;
    QBrush* brush;
    QList<QGraphicsItem*> lastActions;
};



#endif // GENERALCONTROLLER_H
