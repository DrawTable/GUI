#ifndef CAMERAMANAGERUPDATER_H
#define CAMERAMANAGERUPDATER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QObject>

/**
 * @brief This class is design to be launched as a thread by the CameraManager.
 *
 */
class CameraManagerUpdater : public QObject
{
    Q_OBJECT
public:
    /**
     * Start the updater.
     *
     * @param captures List of captures created by the CameraManager.
     *
     */
    CameraManagerUpdater(std::vector<cv::VideoCapture*> captures);

    /**
     * Stop updating live views.
     */
    void stopUpdater();

private:
    std::vector<cv::VideoCapture*> captures;
    bool stopped = false;

public slots:
    void process();

signals:
    void finished();
};

#endif // CAMERAMANAGERUPDATER_H
