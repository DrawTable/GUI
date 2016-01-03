#ifndef THREADCAPTURE
#define THREADCAPTURE

#include <QThread>
#include <QMutex>
#include <QSemaphore>
#include <QWaitCondition>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/**
 * @brief La classe ThreadCapture obient les frames du la caméra
 * dans un thread.
 */
class ThreadCapture: public QThread{

private:
    VideoCapture *cap;
    QMutex mutex;
    Mat lastFrame;
    bool flagRead = true;
    QWaitCondition waitFlag;

public:

    ThreadCapture(int device = 0, QObject *parent = 0) : QThread(parent){
        cap = new VideoCapture(device);
        if(!cap->isOpened()){
            cerr << "[ThreadCapture]: Unable to open camera no: " << device << endl;
        }
    }

    ThreadCapture(const String& filename, QObject *parent = 0) : QThread(parent){
        cap = new VideoCapture(filename);
        if(!cap->isOpened()){
            cerr << "[ThreadCapture]: Unable to open file: " << filename << endl;
        }
    }

    void run() Q_DECL_OVERRIDE{
        waitKey(10);
        int num = 0;
        for(;;){
            mutex.lock();
            if(!flagRead){
                waitFlag.wait(&mutex);
            }
            cap->read(lastFrame);
            flagRead = false;
            mutex.unlock();
            num ++;
            waitKey(1);
            if(isInterruptionRequested()){
                break;
            }
        }
        cout << "Thread num frame read: " << num << endl;
    }

    Mat get(){
        Mat frame;
        mutex.lock();
        lastFrame.copyTo(frame);
        flagRead = true;
        waitFlag.wakeOne();
        mutex.unlock();
        return frame;
    }

    ~ThreadCapture(){
        delete cap;
    }

};

#endif // THREADCAPTURE

