#include "cameramanagerupdater.h"

CameraManagerUpdater::CameraManagerUpdater(std::vector<cv::VideoCapture*> captures)
{
    this->captures = captures;
}

void CameraManagerUpdater::process() {
    while (!stopped) {
        for (unsigned int i = 0; i < captures.size(); i++) {
            cv::VideoCapture* cap = captures.at(i);
            cv::Mat frame;
            cap->read(frame);

            std::string windowName = "Camera #" + std::to_string(i);
            cv::imshow(windowName, frame);
        }
    }

    cv::destroyAllWindows();

    emit finished();
}

void CameraManagerUpdater::stopUpdater() {
    stopped = true;
}
