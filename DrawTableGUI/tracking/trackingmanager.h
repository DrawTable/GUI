#ifndef TRACKINGMANAGER_H
#define TRACKINGMANAGER_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include "screendetector.h"

/**
 * @brief Tracking Manager
 *
 * This is the main manager for all the tracking part.
 * It starts with the screen detection and once it's done, it reads frames from the camera,
 * find the position of the stylus using the LedDetector and mouve the mouse accordingly.
 */
class TrackingManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Create the Tracking Manager
     * @param cameraId ID of the camera the user wants to use.
     */
    explicit TrackingManager(int cameraId, QObject *parent = 0);

private:
    VideoCapture* cap;
    Mat transformMatrix;
    int cameraId;

    void mainLoop();

public slots:
    void process();
    void onStratCalibration(int width, int height);

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
