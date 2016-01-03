#include <threadcapture.h>
#include <ledtracker.h>

void testVideo(){

    VideoCapture cap(1);
    if(!cap.isOpened())
        return;

    waitKey(1);
    LedTracker* lt = LedTracker::getInstance();
    lt->activateDebugAnalysis();
    lt->setObjectType(LedTracker::POINTER);
    Mat frame;

    for(;;){
        cap >> frame;
        if(frame.empty())
        return;

        lt->setFrame(frame);
        lt->processLedAnalysis();

        int key = waitKey(2);
        if(key == (int)'p'){
            waitKey(0);
        }
        else if(key == (int)'c'){
            lt->setMode(LedTracker::CALIBRATION);
        }
        else if(key == (int)'t'){
            lt->setMode(LedTracker::TRACKING);
        }
        else if(key == 27){
            cout << "finished" << endl;
            break;
        }
    }
}


int main()
{

    testVideo();
    return 0;
}

