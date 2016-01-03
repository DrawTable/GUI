#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QObject>
#include <QThread>

#include <QDebug>

#include "cameramanagerupdater.h"

/**
 * @brief The CameraManager detects and lists user's cameras
 *
 * It tries to detect the ten first camera connected to the computer.
 * It also provides a live view of every camera allowing the user to choose which camera he wants to use.
 * When a user clicks on a window, a handler will be triggered and the associated camera is selected.
 *
 * This class is a Singleton.
 */
class CameraManager : public QObject
{
    Q_OBJECT
public:
    static CameraManager* instance;

    /**
     * Get the instance of the Singleton.
     */
    static CameraManager* getInstance();

    /**
     * Init the camera detection. Every camera will then be retain by the CameraManager.
     */
    void initCameras();

    /**
     * Show every camera in separated live views allowing the user to select which one he wants to use.
     */
    void listCameras();

    /**
     * Count how many cameras has been detected by the CameraManager.
     *
     * @return The number of cameras detected.
     */
    int countCameras();

private:
    CameraManager();

    std::vector<cv::VideoCapture*> captures;
    QThread* thread;
    CameraManagerUpdater* updater;

    /**
     * Destroy live views created by this class.
     */
    void destroyWindows();

    /**
     * Callback to clean everything when the update has finished updating windows.
     */
    void onUpdaterFinished();

    /**
     * Emit the chosen camera ID via a Qt signal.
     */
    void emitCameraId(int cameraId);

    /**
     * Launch a thread, the CameraManagerUpdater, to keep the live views refreshing.
     */
    void startUpdater();

    /**
     * Handle click events sent by live windows.
     */
    static void mouseHandler(int, int, int, int, void*);

signals:
    /**
     * Emit the chosen camera ID
     */
    void cameraChoosen(int id);
};

#endif // CAMERAMANAGER_H
