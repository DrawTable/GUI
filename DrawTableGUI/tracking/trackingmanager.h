#ifndef TRACKINGMANAGER_H
#define TRACKINGMANAGER_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include "screendetector.h"

class TrackingManager : public QObject
{
    Q_OBJECT
public:
    explicit TrackingManager(int cameraId, QObject *parent = 0);

private:
    VideoCapture* cap;
    Mat transformMatrix;
    int cameraId;

    void mainLoop();

public slots:
    void process();
    void onStratCalibration(int width, int height);
    void onStartStylusCalibration();

signals:
    void showGreenScreen();
    void calibrationSuccess();
    void stylusCalibrationSuccess();
    void stylusCalibrationError(int errorCode);
    void stylusCalibrationProgress(int);
    void calibrationError(int errorCode);

    void mouseMove(int x, int y);
    void mousePress();
    void mouseRelease();

    void finished();
};

#endif // TRACKINGMANAGER_H
