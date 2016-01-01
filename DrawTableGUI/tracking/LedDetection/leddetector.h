#ifndef LEDDETECTOR
#define LEDDETECTOR

#include <opencv2/opencv.hpp>
#include <iostream>
#include <QString>

using namespace std;
using namespace cv;

// values for led on TV
// 151 47 156
// 5 255 255
class LedDetector {

    Mat orig, img, hsv, thresholded, ledThresholded;
    int lowH = 139, lowS = 0, lowV = 216, highH = 177, highS = 176, highV = 255;
//    int lowH = 131, lowS = 47, lowV = 168, highH = 5, highS = 255, highV = 255;
    int led_lowH = 0, led_lowS = 0, led_lowV = 0, led_highH = 179, led_highS = 255, led_highV = 255;
    int h_bins = 31, s_bins = 16, v_bins = 40;

    bool isSelectingRoi = false, isSelectingHsvRange = false;
    Rect selection;
    Point origin;
    Mat img_selection, hsv_selection;

    int minH = 179, minS = 255, minV= 255, maxH = 0, maxS = 0, maxV = 0;

    Rect lightTracking;
    RNG rng;

    Mat forgroundMask;
    Point ledPos;

    cv::Ptr<BackgroundSubtractorMOG2> bg;

public:

    /////////////////////////// static ////////////////////////////////

    static LedDetector* instance;
    static LedDetector* getInstance(){
        if(instance == nullptr)
            instance = new LedDetector();
        return instance;
    }

    static void onTrackBarChanged(int, void*){
        LedDetector::getInstance()->trackLed();
    }

    static void onLedTrackBarChanged(int, void*){
        LedDetector::getInstance()->trackLed();
    }

    static void onTrackBarChangedSubtractor(int, void*){
        LedDetector::getInstance()->backgroundSubtraction();
    }

    static void onMouse(int event, int x, int y, int, void*){
        LedDetector::getInstance()->selectRoi(event, Point(x,y));
    }

    static void onSelectionMouse(int event, int x, int y, int, void*){
        LedDetector* ld = LedDetector::getInstance();
        if(event == EVENT_LBUTTONDBLCLK)
            ld->calcHistograms(ld->getHsvImgSelection());
        else
            ld->findMouseHsvRange(event,x,y);

    }

    /////////////////////////// public ////////////////////////////////

    void setImage(Mat& img){
        this->img = img;
        this->img.copyTo(orig);
        cvtColor(this->img, hsv, CV_BGR2HSV);
    }

    void backgroundSubtraction(){

        Mat fore;

        // Track tout objet en movement
        bg->apply(this->img,fore, 0.1);


//        cv::erode (fore, fore, cv::Mat ());
//        cv::dilate (fore, fore, cv::Mat ());
        erode(fore, fore, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(fore, fore, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(fore, fore, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(fore, fore, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        // Track tout objet rouge
        // Hue range is circular [0:179] deg
        if(lowH <= highH){
            // min Hue value <= max value (i.e [20:160])
            inRange(hsv, Scalar(lowH, lowS, lowV), Scalar(highH,highS,highV), thresholded);
        } else {
            // min Hue value >= max value (i.e: [160:20])
            vector<Mat> hsv_planes;
            split(hsv,hsv_planes);

            Mat maskH, maskS, maskV;
            inRange(hsv_planes[0], highH, lowH, maskH);    // inRange Hue (max -> min)
            inRange(hsv_planes[1], lowS, highS, maskS);    // inRange Sat (normal)
            inRange(hsv_planes[2], lowV, highV, maskV);    // inRange Val (normal)

            bitwise_and(maskS, maskV, thresholded);        // merge Sat & Val
            bitwise_not(maskH, maskH);                     // invese Hue range
            bitwise_and(maskH, thresholded, thresholded);  // merge All
        }


        erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));


        // mask objet en mouvement
        Mat maskedFore;
        this->orig.copyTo(maskedFore, fore);


        // mask couleur
        Mat maskedThres;
        this->orig.copyTo(maskedThres, thresholded);

        Mat roi;
        bitwise_and(maskedFore, maskedThres, roi);
        vector<Mat> planes;
        split(roi, planes);
        roi = planes[1];

        vector<vector<Point>> contours;
        vector<Point> approxCircle;


        Mat contoured;
        roi.copyTo(contoured);
        findContours(contoured, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        long nbContours = contours.size();
        if(nbContours > 0){
            for(int i=0; i < nbContours; i++){
                approxPolyDP(contours.at(i), approxCircle, 0.01 * arcLength(contours.at(i), true), true);
                Scalar color(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
                polylines(img, approxCircle, true, color, 2);
                putText(img, to_string(approxCircle.size()), Point(approxCircle.at(0).x - 30, approxCircle.at(0).y - 30), FONT_HERSHEY_SIMPLEX, 1, color, 2);
            }
        }


        cv::imshow ("Fore", fore);
        cv::imshow ("Frame", this->img);
        cv::imshow ("Masked", maskedFore);
        cv::imshow ("threshold", maskedThres);
        cv::imshow("roi", roi);
    }

    void activateDebugBgSubstraction(){
        cv::namedWindow ("Frame", 0);
        cv::namedWindow ("Fore", 0);
        cv::namedWindow ("Masked", 0);
        cv::namedWindow("threshold", 0);
        cv::namedWindow("roi", 0);

        lowH = 139;
        lowS = 0;
        lowV = 139;

        highH = 4;
        highS = 255;
        highV = 255;

        createTrackbar("lowH", "threshold", &lowH, 179, onTrackBarChangedSubtractor);
        createTrackbar("lowS", "threshold", &lowS, 255, onTrackBarChangedSubtractor);
        createTrackbar("lowV", "threshold", &lowV, 255, onTrackBarChangedSubtractor);
        createTrackbar("highH", "threshold", &highH, 179, onTrackBarChangedSubtractor);
        createTrackbar("highS", "threshold", &highS, 255, onTrackBarChangedSubtractor);
        createTrackbar("highV", "threshold", &highV, 255, onTrackBarChangedSubtractor);
    }

    Point debugLedDetection(){
        namedWindow("source", 0);
        namedWindow("threshold", 0);
        namedWindow("ledThresholded", 0);
        namedWindow("masked", 0);
        setMouseCallback("source", onMouse);
        setMouseCallback("threshold", onMouse);

        createTrackbar("lowH", "threshold", &lowH, 179, onTrackBarChanged);
        createTrackbar("lowS", "threshold", &lowS, 255, onTrackBarChanged);
        createTrackbar("lowV", "threshold", &lowV, 255, onTrackBarChanged);
        createTrackbar("highH", "threshold", &highH, 179, onTrackBarChanged);
        createTrackbar("highS", "threshold", &highS, 255, onTrackBarChanged);
        createTrackbar("highV", "threshold", &highV, 255, onTrackBarChanged);

        createTrackbar("Led lowH", "ledThresholded", &led_lowH, 179, onLedTrackBarChanged);
        createTrackbar("Led lowS", "ledThresholded", &led_lowS, 255, onLedTrackBarChanged);
        createTrackbar("Led lowV", "ledThresholded", &led_lowV, 255, onLedTrackBarChanged);
        createTrackbar("Led highH", "ledThresholded", &led_highH, 179, onLedTrackBarChanged);
        createTrackbar("Led highS", "ledThresholded", &led_highS, 255, onLedTrackBarChanged);
        createTrackbar("Led highV", "ledThresholded", &led_highV, 255, onLedTrackBarChanged);
        trackLed();

        return ledPos;
    }

    void trackLed(){
        trackLedLight();
        trackLedCenter();
    }

    void trackLedCenter(){

        if(checkLightTrackingRect()){
            Mat LedRoi = hsv(lightTracking);
            inRange(LedRoi, Scalar(led_lowH, led_lowS, led_lowV), Scalar(led_highH, led_highS, led_highV), ledThresholded);
        }
        showImages();
    }

    void trackLedLight(){

        // Hue range is circular [0:179] deg
        if(lowH <= highH){
            // min Hue value <= max value (i.e [20:160])
            inRange(hsv, Scalar(lowH, lowS, lowV), Scalar(highH,highS,highV), thresholded);
        } else {
            // min Hue value >= max value (i.e: [160:20])
            vector<Mat> hsv_planes;
            split(hsv,hsv_planes);

            Mat maskH, maskS, maskV;
            inRange(hsv_planes[0], highH, lowH, maskH);    // inRange Hue (max -> min)
            inRange(hsv_planes[1], lowS, highS, maskS);    // inRange Sat (normal)
            inRange(hsv_planes[2], lowV, highV, maskV);    // inRange Val (normal)

            bitwise_and(maskS, maskV, thresholded);        // merge Sat & Val
            bitwise_not(maskH, maskH);                     // invese Hue range
            bitwise_and(maskH, thresholded, thresholded);  // merge All
        }

        erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        if(selection.area() > 0)
            calcHistograms(hsv_selection);


        // find Led light bounding rect
        vector<vector<Point>> contours;
        findLedPosMaxRedArea(contours);
        drawCross(img, ledPos, Scalar(0,255,0), 2);

    }

    void drawCross(Mat &frame, Point center, Scalar color, int size = 1){
        line(frame, Point(center.x, center.y -size), Point(center.x,center.y + size ), color, size);
        line(frame, Point(center.x-size, center.y), Point(center.x + size,center.y), color, size);
    }

    void findLedPosMaxRedArea(vector<vector<Point>> contours){

        Mat threshold;
        thresholded.copyTo(threshold);
        findContours(threshold, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        if(contours.size() == 0) {
            ledPos = Point(-1,-1);
            return;
        }

        sort(contours.begin(), contours.end(), [&](const std::vector<cv::Point> c1, const std::vector<cv::Point> c2){
            return contourArea(c1) > contourArea(c2);
        });


        Moments oMoments = moments(contours.at(0));
        double m01 = oMoments.m01;
        double m10 = oMoments.m10;
        double mArea = oMoments.m00;

        if(mArea < 20){
            ledPos = Point(-1,-1);
            return;
        }


        ledPos.x = m10/mArea;
        ledPos.y = m01/mArea;
    }

    Point findLedPosMergeRed(vector<vector<Point>> contours){

        Mat threshold;
        thresholded.copyTo(threshold);
        vector<Point> points;
        findContours(threshold, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        if(contours.size() > 0){
            for(int i= 0; i < contours.size(); ++i){
                for(int j =0; j < contours.at(i).size(); ++j)
                    points.push_back(contours.at(i).at(j));
            }

            vector<Point> hull;
            convexHull(points, hull, true, true);
            lightTracking =  minAreaRect(hull).boundingRect();
            int add_width = cvRound(lightTracking.width*0.3);
            int add_height = cvRound(lightTracking.height*0.3);
            lightTracking.width += add_width;
            lightTracking.height += add_height;
            lightTracking.x -= cvRound(add_width/2.0);
            lightTracking.y -= cvRound(add_height/2.0);

            ledPos = Point(lightTracking.x + (lightTracking.width/2), lightTracking.y + (lightTracking.height/2));

            if(checkLightTrackingRect())
                rectangle(img, lightTracking, Scalar(0,255,0), 2);
        } else {
            ledPos = Point(-1,-1);
        }

        return ledPos;
    }

    bool checkLightTrackingRect(){
        return 0 <= lightTracking.x
                && 0 <= lightTracking.width
                && lightTracking.x + lightTracking.width <= img.cols
                && 0 <= lightTracking.y
                && 0 <= lightTracking.height
                && lightTracking.y + lightTracking.height <= img.rows;
    }

    void showImages(){
        imshow("source", img);
//        imshow("threshold", thresholded);

        Mat masked;
        orig.copyTo(masked, thresholded);
        imshow("masked", masked);

//        if(!ledThresholded.empty() && checkLightTrackingRect()){
//            Mat masked, orig_roi;
//            orig_roi = orig(lightTracking);
//            orig_roi.copyTo(masked, ledThresholded);
//            imshow("ledThresholded", masked);
//        }
    }

    void selectRoi(int event, Point p){

        if(isSelectingRoi){
            selection.x = MIN(p.x, origin.x);
            selection.y = MIN(p.y, origin.y);
            selection.width = abs(p.x - origin.x);
            selection.height = abs(p.y - origin.y);

            orig.copyTo(img);
            rectangle(img, selection, Scalar(0,255,0),1);
            imshow("source", img);
        }

        switch (event) {
        case EVENT_LBUTTONDOWN:
            origin = p;
            selection = Rect(p,p);
            isSelectingRoi = true;
            break;
        case EVENT_LBUTTONUP:
            isSelectingRoi = false;
            if(selection.area() > 0){
                handleSelection();
            } else {
                destroyWindow("selection");
                destroyWindow("h_hist");
                destroyWindow("s_hist");
                destroyWindow("v_hist");
            }
            break;
        }
    }

    void findMouseHsvRange(int event, int x, int y){

        switch (event) {
        case EVENT_LBUTTONDOWN:
            // start selecting range
            isSelectingHsvRange = true;
            break;
        case EVENT_LBUTTONUP:
            // stop selecting range
            isSelectingHsvRange = false;
            cout << "H: " << "[" << minH << ":" << maxH << "]\n"
                 << "S: " << "[" << minS << ":" << maxS << "]\n"
                 << "V: " << "[" << minV << ":" << maxV << "]\n";

            minH = 179, minS = 255, minV= 255, maxH = 0, maxS = 0, maxV = 0;
            break;
        }

        if(isSelectingHsvRange){
            Vec3b pixel = hsv_selection.at<Vec3b>(Point(x,y));
            int H = pixel[0];
            int S = pixel[1];
            int V = pixel[2];
            cout << "current H: " << H << " S: " << S << " V: " << V << endl;

            minH = H < minH ? H: minH;
            minS = S < minS ? S: minS;
            minV = V < minV ? V: minV;

            maxH = H > maxH ? H: maxH;
            maxS = S > maxS ? S: maxS;
            maxV = V > maxV ? V: maxV;
        }
    }

    void handleSelection(){
        namedWindow("selection", 0);            // show window selection
        setMouseCallback("selection", onSelectionMouse);
        img_selection = orig(selection);         // get the region of interset
        hsv_selection = hsv(selection);
        imshow("selection", img_selection);
    }

    void calcHistograms(Mat hsv_img){
        // split hsv channels
        vector<Mat> hsv_planes;
        split(hsv_img, hsv_planes);
        int bins[] = {h_bins,s_bins,v_bins};
        float range256[] = {0,256};
        float range179[] = {0,179};
        const float *h_range = {range179}, *s_range = {range256}, *v_range = {range256};

        MatND h_hist, s_hist, v_hist;
        bool uniform = true, accumulate = false;

        calcHist(&hsv_planes[0], 1, 0, Mat(), h_hist, 1, &bins[0], &h_range, uniform, accumulate);
        calcHist(&hsv_planes[1], 1, 0, Mat(), s_hist, 1, &bins[1], &s_range, uniform, accumulate);
        calcHist(&hsv_planes[2], 1, 0, Mat(), v_hist, 1, &bins[2], &v_range, uniform, accumulate);

        namedWindow("h_hist", 0);
        namedWindow("s_hist", 0);
        namedWindow("v_hist", 0);
        int W = 512; int H = 400;
        int h_bin_w = cvRound( (double) W/bins[0]);
        int s_bin_w = cvRound( (double) W/bins[1]);
        int v_bin_w = cvRound( (double) W/bins[2]);

        cv::normalize(h_hist, h_hist, 0, H, NORM_MINMAX, -1, Mat());
        cv::normalize(s_hist, s_hist, 0, H, NORM_MINMAX, -1, Mat());
        cv::normalize(v_hist, v_hist, 0, H, NORM_MINMAX, -1, Mat());

        Mat img_h_hist = Mat::zeros(H, W, CV_8UC3),
            img_s_hist = Mat::zeros(H, W, CV_8UC3),
            img_v_hist = Mat::zeros(H, W, CV_8UC3);

        for(int i=1; i < bins[0]; ++i){
            Scalar color(255,0,0);
            rectangle(img_h_hist,
                      Point((i-1)*h_bin_w, H),
                      Point(i*h_bin_w, H - cvRound(h_hist.at<float>(i-1))),
                      color, CV_FILLED);
        }

        for(int i=1; i < bins[1]; ++i){
            Scalar color(0,255,0);
            rectangle(img_s_hist,
                      Point((i-1)*s_bin_w, H),
                      Point(i*s_bin_w, H - cvRound(s_hist.at<float>(i-1))),
                      color, CV_FILLED);
        }

        for(int i=1; i < bins[2]; ++i){
            Scalar color(0,0,255);
            rectangle(img_v_hist,
                      Point((i-1)*v_bin_w, H),
                      Point(i*v_bin_w, H - cvRound(v_hist.at<float>(i-1))),
                      color, CV_FILLED);
        }


        line(img_h_hist, Point( W * lowH / 179.0, 0), Point(W * lowH / 179.0, H-1), Scalar(255,255,0), 1);
        line(img_h_hist, Point( W * highH / 179.0, 0), Point(W * highH / 179.0, H-1), Scalar(255,255,0), 1);

        line(img_s_hist, Point( W * lowS / 255.0, 0), Point(W * lowS / 255.0, H-1), Scalar(255,255,0), 1);
        line(img_s_hist, Point( W * highS / 255.0, 0), Point(W * highS / 255.0, H-1), Scalar(255,255,0), 1);

        line(img_v_hist, Point( W * lowV / 255.0, 0), Point(W * lowV / 255.0, H-1), Scalar(255,255,0), 1);
        line(img_v_hist, Point( W * highV / 255.0, 0), Point(W * highV / 255.0, H-1), Scalar(255,255,0), 1);


        imshow("h_hist", img_h_hist);
        imshow("s_hist", img_s_hist);
        imshow("v_hist", img_v_hist);
    }

    ///////////////////////// Getter&Setter ////////////////////////////

    Mat getHsvImgSelection(){ return hsv_selection; }

private:

    /////////////////////////// private ////////////////////////////////
    LedDetector(){
        bg =  createBackgroundSubtractorMOG2();
        bg->setNMixtures(3);
        bg->setDetectShadows(false);
    }

    ~LedDetector(){
    }

};

#endif // LEDDETECTOR

