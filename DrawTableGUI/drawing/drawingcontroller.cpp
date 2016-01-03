#include "drawingcontroller.h"
#include "../tool/pencontroller.h"
#include <QDebug>

DrawingController::DrawingController(Drawing* drawing) {
    this->drawing = drawing;
    this->drawController = PenController::getInstance();
    this->pen = new QPen(Qt::white);
    this->drawing->setController(this);
}

DrawingController::~DrawingController() {
}

void DrawingController::enable() {
    isEnable = true;
}

void DrawingController::disable() {
    isEnable = false;
}

void DrawingController::undo() {
    if (canUndo()) {
        QGraphicsItem* temp = lastActions.takeLast();
        nextActions.append(temp);
        drawing->scene()->removeItem(temp);
        modifToSave = true;
    }
}

void DrawingController::redo() {
    if(canRedo()){
        QGraphicsItem* temp = nextActions.takeLast();
        lastActions.append(temp);
        drawing->scene()->addItem(temp);
        modifToSave = true;
    }
}

void DrawingController::resetUndoHistory() {
    lastActions.clear();
    nextActions.clear();
}

void DrawingController::mouseDoubleClickEvent(QMouseEvent* event) {
    if (isEnable) {
        drawController->mouseDoubleClickEvent(drawing->scene(), event);
    }
}

void DrawingController::mouseMoveEvent(QMouseEvent* event) {
    if (isEnable && event->buttons() == Qt::LeftButton) {
        drawController->mouseMoveEvent(drawing->scene(), event);
    }
}

void DrawingController::mousePressEvent(QMouseEvent* event) {
    if (isEnable) {
        drawController->mousePressEvent(drawing->scene(), event, pen);
    }
}

void DrawingController::mouseReleaseEvent(QMouseEvent* event) {
    if (isEnable) {
        lastActions.append(drawController->mouseReleaseEvent(drawing->scene(), event));
        nextActions.clear();
        modifToSave = true;
    }
}

bool DrawingController::canRedo(){
    return !nextActions.isEmpty();
}



bool DrawingController::canUndo(){
    return !lastActions.isEmpty();
}

//------------------------------------------------------------
//------------------- GETTERS AND SETTERS --------------------
//------------------------------------------------------------

void DrawingController::setPen(QPen* pen) {
    this->pen = pen;
}

QPen* DrawingController::getPen() {
    return pen;
}

void DrawingController::setBrush(QBrush* brush) {
    this->brush = brush;
}

QBrush* DrawingController::getBrush() {
    return brush;
}

void DrawingController::setDrawController(AbstractToolController* drawController) {
    this->drawController = drawController;
}

AbstractToolController* DrawingController::getDrawController() {
    return drawController;
}

bool DrawingController::toSave()
{
    return modifToSave;
}

void DrawingController::setToSave(bool toSave)
{
    this->modifToSave = toSave;
}
