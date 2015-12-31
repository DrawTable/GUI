#ifndef CONTROLLER
#define CONTROLLER

#include <QThread>
#include <QObject>
#include "worker.h"

class Controller: public QObject{
    Q_OBJECT

    QThread thread;
    Worker* worker;

public:

    Controller();
    void start();

public slots:
signals:
    void mouseMove(int x, int y);
    void mousePressed();
    void mouseReleased();
};
#endif // CONTROLLER

