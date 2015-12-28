#ifndef TRACKINGMANAGER_H
#define TRACKINGMANAGER_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include "screendetector.h"

class TrackingManager : public QObject
{
    Q_OBJECT
public:
    explicit TrackingManager(QObject *parent = 0);

private:
    VideoCapture* cap;

    void mainLoop();

public slots:
    void process();
    void onStratCalibration();

signals:
    void showGreenScreen();
    void calibrationSuccess();
    void calibrationError(int errorCode);

    void mouseMove(int x, int y);
    void mousePress();
    void mouseRelease();

    void finished();
};

#endif // TRACKINGMANAGER_H
