#include "screendetector.h"

ScreenDetector::ScreenDetector(Mat img, int width, int height)
    :img(img)
{
    hsvMin = Scalar(41,95,50);
    hsvMax = Scalar(90,255,255);
    this->interfaceWidth = width;
    this->interfaceHeight = height;
}

void ScreenDetector::setColorRange(const Scalar hsvMin, const Scalar HsvMax)
{
    this->hsvMin = hsvMin;
    this->hsvMax = HsvMax;
}

Mat ScreenDetector::getTransformationMatrix(Error& error)
{
    bool approxFound = false;

    // convert image to HSV
    cvtColor(img, hsv, CV_BGR2HSV);

    // threshold the image
    inRange(hsv, hsvMin, hsvMax, thresholded);

    // Optimize threshold by reducing noise
    erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilateSize, erodeDilateSize)) );
    dilate( thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilateSize, erodeDilateSize)) );
    dilate( thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilateSize, erodeDilateSize)) );
    erode(thresholded, thresholded, getStructuringElement(MORPH_ELLIPSE, Size(erodeDilateSize, erodeDilateSize)) );
    GaussianBlur(thresholded, thresholded, Size(3,3), 0);

    Mat forContours;
    thresholded.copyTo(forContours);
    // find all contours
    Contours contours;
    Contour approximatedScreen;
    findContours(forContours, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    int nbContours = contours.size();
    cout << nbContours << " contours found, debug: " << DEBUG << endl;

    if(nbContours == 0)
    {
        error.setError("Unable to find the screen",
                       "The camera doesn't detect any screen or green element."
                       "Please check if your screen is turned on and directed toward the screen");
        return img;
    }

    sort(contours.begin(), contours.end(), contour_compare_area);

    // find the contour with the biggest area that have 4 points when approximated
    for(int i=0; i < nbContours; ++i)
    {
        approxPolyDP(contours.at(i), approximatedScreen, approximateEpsilon * arcLength(contours.at(i), true), true);
        // our screen has 4 point when approximated
        if(approximatedScreen.size() == 4)
        {
            approxFound = true;
            break;
        }
    }

    if(!approxFound)
    {
        error.setError("Unable to find the screen properly",
                       "It seems that the screen is not fully detectable by the camera. Try to reduce light in your room");
        return img;
    }

    if(DEBUG)
    {
        namedWindow("debug", WINDOW_KEEPRATIO);
        namedWindow("thresholded_calibration", WINDOW_KEEPRATIO);
        Mat debug = Mat::zeros(img.rows, img.cols, CV_8UC3);
        polylines(debug, approximatedScreen, true, Scalar(0,0,255), 3);
        imshow("debug", debug);
        imshow("thresholded_calibration", thresholded);
    }

    return transformImage(approximatedScreen);
}

Mat ScreenDetector::transformImage(std::vector<Point> rect)
{
    Point tl,tr, bl, br;
    int width, height;
    // order Points : top-left, top-right, bottom-left, bottom-right
    rect = getOrderedPoints(rect);
    tl = rect.at(0);
    tr = rect.at(1);
    br = rect.at(2);
    bl = rect.at(3);

    Point2f src[4];
    src[0] = rect.at(0);
    src[1] = rect.at(1);
    src[2] = rect.at(2);
    src[3] = rect.at(3);

    Point2f dst[4];

    dst[0] = Point(0,0);
    dst[1] = Point(interfaceWidth-1, 0);
    dst[2] = Point(interfaceWidth-1, interfaceHeight-1);
    dst[3] = Point(0, interfaceHeight-1);

    Mat transformMatrix;
    transformMatrix = getPerspectiveTransform(src, dst);

    if(DEBUG)
    {
        Mat warped;
        namedWindow("warped", WINDOW_KEEPRATIO);
        warpPerspective(img, warped, transformMatrix, Size(interfaceWidth, interfaceHeight));
        imshow("warped", warped);
    }

    return transformMatrix;
}

Point ScreenDetector::transformPoint(Point point, Mat transformMatrix)
{
    vector<Point2f> src;
    vector<Point2f> dst;
    src.push_back(point);
    perspectiveTransform(src, dst, transformMatrix);
    return dst.at(0);
}


std::vector<Point> ScreenDetector::getOrderedPoints(std::vector<Point> rect)
{
    Point tl, tr, bl, br;
    sort(rect.begin(), rect.end(), compare_points);
    tl = rect.at(0);
    br = rect.at(3);
    int diff1 = tl.y - rect.at(1).y;
    int diff2 = tl.y - rect.at(2).y;

    if(diff1 < diff2)
    {
        tr = rect.at(2);
        bl = rect.at(1);
    }
    else
    {
        tr = rect.at(1);
        bl = rect.at(2);
    }

    rect.at(0) = tl;
    rect.at(1) = tr;
    rect.at(2) = br;
    rect.at(3) = bl;

    return rect;
}

bool ScreenDetector::contour_compare_area(const std::vector<cv::Point> c1, const std::vector<cv::Point> c2)
{
    return contourArea(c1) > contourArea(c2);
}

bool ScreenDetector::compare_points(const cv::Point& p1, const cv::Point& p2)
{
    return (p1.x + p1.y) < (p2.x + p2.y);
}



