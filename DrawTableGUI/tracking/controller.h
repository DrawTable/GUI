#ifndef CONTROLLER
#define CONTROLLER

#include <QThread>
#include <QObject>
#include "worker.h"

/**
 * @brief Mouse Controller
 * Controller for the tracking manager to move and click with the mouse.
 */
class Controller: public QObject{
    Q_OBJECT

    QThread thread;
    Worker* worker;

public:

    Controller();

    /**
     * @brief Start the mouse controller.
     */
    void start();

public slots:
signals:
    void mouseMove(int x, int y);
    void mousePressed();
    void mouseReleased();
};
#endif // CONTROLLER

