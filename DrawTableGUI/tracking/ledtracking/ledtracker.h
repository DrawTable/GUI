#ifndef LEDTRACKER
#define LEDTRACKER

#include <opencv2/opencv.hpp>
#include <QDebug>

#define DEBUG_MAIN false  // show original and foreground
#define DEBUG false       // show histograms, threshold, etc..

#define NUM_CHANNELS_HIST 3
#define RANGE_BIN_SIZE 2

#define MAX_HUE 179
#define MAX_SAT 255
#define MAX_VAL 255
#define DEFAULT_MAX_AREA 3000

#define Q1 0.1
#define MEDIAN 0.5
#define Q3 0.9

using namespace std;
using namespace cv;

/**
 * @brief La classe LedTracker detect une led sur une image donnée
 * et renvoie les coordonnées (x,y) de sa position sur l'image.
 *
 * Cette classe effectue le calibrage de la couleur de la led à détecter
 * en construisant un modèle probalistic. Les paramètres de ce modèle
 * sont :
 *
 *     1. La distribution des couleurs de la LED (Couleur, saturation, brillance)
 *     2. La Taille de la Led
 *
 * Ce calibrage sert à ne pas confondre la LED avec un element qui lui ressemble.
 */

class LedTracker
{

public:
    enum MODE {NONE, CALIBRATION, TRACKING};
    enum OBJECT_TYPE {POINTER, STYLUS};
    enum TRACKING_METHOD {FAST, PRECISION};
private:

    MODE current_mode = MODE::NONE;
    OBJECT_TYPE object_type = OBJECT_TYPE::STYLUS;
    TRACKING_METHOD tracking_method = FAST;

    struct Params
    {
        int lowH;
        int highH;
        Scalar low_mask_people;
        Scalar high_mask_people;
    } default_stylus_params;

    static LedTracker* instance;

    Mat original, image, hsv, thresholded;
    Mat mask_roi, mask_hist;
    Rect rect_mask_roi;

    int lowH = 0, lowS = 0, lowV = 0, highH = 179, highS = 255, highV = 255;


    // Suppression de fond
    // Isole les éléments ayant apparu ou bougé depuis la dernière image
    Ptr<BackgroundSubtractorMOG2> backgroundSubtractor;
    double learningRate = 0.7;
    Mat foreground;

    // modeles probabilistes de la LED à détecter
    class ModelHistogram
    {

        LedTracker* ledTracker;

        MatND h_hist_model,
              s_hist_model,
              v_hist_model;

        unsigned long nbSampleInModel = 0;
        const int nbSampleRequired = 50;

        MatND h_hist_buffer,
              s_hist_buffer,
              v_hist_buffer;

        Mat h_hist_image,
            s_hist_image,
            v_hist_image;

        vector<Mat> orig_hist_images;

        int h_bins = 30,
            s_bins = 32,
            v_bins = 32;

        double h_median =0, s_median =0, v_median =0;

        int bins[NUM_CHANNELS_HIST] = {h_bins, s_bins, v_bins};
        bool uniform = true, accumulate = true;

        float range_hue[RANGE_BIN_SIZE] = {0, MAX_HUE};
        float range_sat[RANGE_BIN_SIZE] = {0, MAX_SAT};
        float range_val[RANGE_BIN_SIZE] = {0, MAX_VAL};

        const float *h_range = {range_hue},
                     *s_range = {range_sat},
                      *v_range = {range_val};

        vector<Mat> hsv_planes;

        const int W, H;
        int h_bin_w;
        int s_bin_w;
        int v_bin_w;

        Mat estimatedLed;
        int minArea = DEFAULT_MAX_AREA;
        int maxArea = 0;
        int minWhiteArea = DEFAULT_MAX_AREA;
        int maxWhiteArea = 0;
        const int alpha_area = 0.9;

    public:

        ModelHistogram(LedTracker* parent):
            ledTracker(parent), // parent
            W(512),H(400)       // image size
        {
            ///// Init values to represent histogramms in an image
            // bins width
            h_bin_w = cvRound( (double) W/bins[0]);
            s_bin_w = cvRound( (double) W/bins[1]);
            v_bin_w = cvRound( (double) W/bins[2]);

            orig_hist_images.resize(NUM_CHANNELS_HIST);
        }

        int getNbSampleRequired()
        {
            return nbSampleRequired;
        }

        int getNbSampleInModel()
        {
            return nbSampleInModel;
        }

        void calcHistograms(Mat hsv_img, Mat mask = Mat(), bool cumulate_in_model = false)
        {

            Mat saved_h_hist_model,saved_s_hist_model,saved_v_hist_model;
            Mat *h_hist, *s_hist, *v_hist;

            // store values in the probability model of the object to track
            if(cumulate_in_model)
            {
                h_hist = &h_hist_model;
                s_hist = &s_hist_model;
                v_hist = &v_hist_model;

                // use temp histograms
            }
            else
            {
                h_hist = &h_hist_buffer,
                s_hist = &s_hist_buffer,
                v_hist = &v_hist_buffer;
            }

            // split img to vector hue, saturation, values
            split(hsv_img, hsv_planes);

            // save current hist
            if(cumulate_in_model)
            {
                h_hist_model.copyTo(saved_h_hist_model);
                s_hist_model.copyTo(saved_s_hist_model);
                v_hist_model.copyTo(saved_v_hist_model);
            }

            calcHist(&hsv_planes[0], 1, 0, mask, *h_hist, 1, &bins[0], &h_range, uniform, cumulate_in_model);
            calcHist(&hsv_planes[1], 1, 0, mask, *s_hist, 1, &bins[1], &s_range, uniform, cumulate_in_model);
            calcHist(&hsv_planes[2], 1, 0, mask, *v_hist, 1, &bins[2], &v_range, uniform, cumulate_in_model);

            if(cumulate_in_model)
            {
                double correlation = 0;
                bool correlationOk = false;

                if(nbSampleInModel != 0)
                {
                    correlation += compareHist(saved_h_hist_model, h_hist_model, CV_COMP_CORREL);
                    correlation += compareHist(saved_s_hist_model, s_hist_model, CV_COMP_CORREL);
                    correlation += compareHist(saved_v_hist_model, v_hist_model, CV_COMP_CORREL);
                    correlationOk = (correlation > 2.0 && correlation < 2.9999999);

//                    Rect roi_bounding_rect = Rect(0,0,hsv_img.rows, hsv_img.cols);
                    Rect roi_bounding_rect = ledTracker->getMaxBoundingRect(mask);
                    int area = roi_bounding_rect.area();
                    int whiteArea = ledTracker->getWhiteAreaOnRoi();
                    minArea = area < minArea ? area : minArea;
                    maxArea = area > maxArea ? area : maxArea;
                    minWhiteArea = whiteArea < minWhiteArea ? whiteArea : minWhiteArea;
                    maxWhiteArea = whiteArea > maxWhiteArea ? whiteArea : maxWhiteArea;

                }
                else
                {
                    correlationOk = true;
                }

                if(!correlationOk)
                {
                    h_hist_model = Mat::zeros(h_hist_model.rows, h_hist_model.cols, h_hist_model.type());
                    s_hist_model = Mat::zeros(s_hist_model.rows, s_hist_model.cols, s_hist_model.type());
                    v_hist_model = Mat::zeros(v_hist_model.rows, v_hist_model.cols, v_hist_model.type());
                    nbSampleInModel = 0;
                    minArea = DEFAULT_MAX_AREA;
                    maxArea = 0;
                    minWhiteArea = DEFAULT_MAX_AREA;
                    maxArea = 0;
                    qDebug("correlation too low (%f), reset model", correlation);
                    return;
                }
                else
                {
                    nbSampleInModel++;
                    qDebug("%d/%d samples (%f) size [%d - %d] white [%d, %d]", nbSampleInModel, nbSampleRequired,correlation, minArea, maxArea, minWhiteArea, maxWhiteArea);
                }
            }

            Mat h_hist_cumul, s_hist_cumul, v_hist_cumul;
            processHistogramAnalysis(h_hist, s_hist, v_hist,
                                     &h_hist_cumul, &s_hist_cumul, &v_hist_cumul, &hsv_img, &mask);

            if(DEBUG)
            {
                showHistograms(h_hist_cumul, s_hist_cumul, v_hist_cumul);
            }
        }

        void processHistogramAnalysis(Mat *h_hist, Mat *s_hist, Mat *v_hist,
                                      Mat *h_hist_cumul, Mat *s_hist_cumul, Mat *v_hist_cumul, Mat* hsv_img = nullptr, Mat* mask = nullptr )
        {

            if(h_hist->empty() || s_hist->empty() || v_hist->empty()) return;

            Vec2i acceptable_hue_range, acceptable_sat_range, acceptable_val_range;
//            Vec2f probability_range_hue(0.2,0.9), probability_range_sat(0.08, -1), probability_range_val(0.08, -1);
            Vec2f probability_range_hue(0.2,0.9), probability_range_sat(0.15,0.9), probability_range_val(0.2,0.9);

            normalizeSum(h_hist, h_hist_cumul, acceptable_hue_range, probability_range_hue, Range(0,6));
            normalizeSum(s_hist, s_hist_cumul, acceptable_sat_range, probability_range_sat, Range(0,4));
            normalizeSum(v_hist, v_hist_cumul, acceptable_val_range, probability_range_val);

            double bin_width_hue = (range_hue[1] + 1)/(double)bins[0];
            double bin_width_sat = (range_sat[1] + 1)/(double)bins[1];
            double bin_width_val = (range_val[1] + 1)/(double)bins[2];

            if(ledTracker->object_type == STYLUS)
            {
                ledTracker->lowH = ledTracker->default_stylus_params.lowH;
                ledTracker->highH = ledTracker->default_stylus_params.highH;
            }
            else
            {
                ledTracker->lowH = (acceptable_hue_range[0] * bin_width_hue);
                ledTracker->highH = (acceptable_hue_range[1] * bin_width_hue);
            }

            ledTracker->lowS = (acceptable_sat_range[0] * bin_width_sat);
            ledTracker->highS = (acceptable_sat_range[1] * bin_width_sat) ;

            ledTracker->lowV = acceptable_val_range[0] * bin_width_val;
            ledTracker->highV = acceptable_val_range[1] * bin_width_val;

            h_median = 0;
            s_median = 0;
            v_median = 0;

            if(hsv_img && mask && ledTracker->object_type != STYLUS)
            {
                Mat customHist;
                customHist = Mat::zeros(180,1, h_hist->type());
                for(int i=0; i < hsv_img->rows * hsv_img->cols; ++i)
                {
                    Vec3b pixel = hsv_img->at<Vec3b>(i);
                    if(pixel[1] >= ledTracker->lowS && pixel[1] <= ledTracker->highS)
                    {
                        customHist.at<float>(pixel[0]) += 1;
                    }
                }

                Vec2i new_hue_range;
                Vec2f range_hue(0.01,0.09);
                normalizeSum(&customHist, &customHist, new_hue_range, range_hue);
                cout << "Estimated Hue Range: [" << ledTracker->lowH << "," <<  ledTracker->highH << "]" << endl;
                cout << "Improved Hue Range: " << new_hue_range << endl;
                ledTracker->lowH = MIN(ledTracker->lowH, new_hue_range[0]);
                ledTracker->highH = MAX(ledTracker->highH, new_hue_range[1]);
            }

            if(DEBUG)
            {
                qDebug(" Hue: [%d,%d]", ledTracker->lowH, ledTracker->highH);
                qDebug(" Sat: [%d,%d]", ledTracker->lowS, ledTracker->highS);
                qDebug(" Val: [%d,%d] \n", ledTracker->lowV, ledTracker->highV);
            }
        }

        void normalizeSum(Mat *hist, Mat *dest, Vec2i &bin_ranges, Vec2f probability_min, Range thres = Range(0,0))
        {
            long histSize= hist->total();
            if(dest != hist &&
                    (dest->total() != hist->total()|| dest->type() != hist->type()))
            {
                dest->create(hist->rows, hist->cols, hist->type());
            }

            bool rangeMinFound = false, rangeMaxFound = false;
            unsigned long sum = 0;

            for(int i=0; i < histSize; ++i)
            {
                if(!thres.empty() && (i >= thres.start && i <= thres.end))
                {
                    hist->at<float>(i) = 0;
                }

                sum += (unsigned long)hist->at<float>(i);
            }

            for(int i=0; i < histSize; ++i)
            {
                float hist_val = hist->at<float>(i);
                double new_value = 0;
                if(hist_val > 0)
                {
                    new_value =  hist_val/double(sum);
                }

                new_value += (i > 0 ? dest->at<float>(i-1): 0);
                dest->at<float>(i) = new_value;

                if(new_value >= probability_min[0] && !rangeMinFound)
                {
                    bin_ranges[0] = i + 1;
                    rangeMinFound = probability_min[0] != -1;
                }

                if(new_value >= probability_min[1] && !rangeMaxFound)
                {
                    bin_ranges[1] = i + 1;
                    rangeMaxFound = probability_min[1] != -1;
                }

                if(bin_ranges[0] == bin_ranges[1])
                {
                    bin_ranges[1]++;
                }
            }
        }

        void showModelHist()
        {
            Mat h_hist_cumul, s_hist_cumul, v_hist_cumul;
            processHistogramAnalysis(&h_hist_model, &s_hist_model,&v_hist_model, &h_hist_cumul, &s_hist_cumul, &v_hist_cumul);

            if(DEBUG)
                showHistograms(h_hist_cumul, s_hist_cumul, v_hist_cumul);
        }

        void showHistograms(Mat h_hist, Mat s_hist, Mat v_hist)
        {

            // init images
            for(int i=0; i < orig_hist_images.size(); ++i)
            {
                Mat buffer;
                buffer = Mat::zeros(H,W, CV_8UC3);
                orig_hist_images[i] = buffer;
            }

            // draw hue histogram
            for(int i=1; i < bins[0]; ++i)
            {
                Scalar color(255,0,0);
                rectangle(orig_hist_images[0],
                          Point((i-1)*h_bin_w, H),
                          Point(i*h_bin_w, H - cvRound(H*h_hist.at<float>(i-1))),
                          color, CV_FILLED);
            }

            // draw saturation histogram
            for(int i=1; i < bins[1]; ++i)
            {
                Scalar color(0,255,0);
                rectangle(orig_hist_images[1],
                          Point((i-1)*s_bin_w, H),
                          Point(i*s_bin_w, H - cvRound(H*s_hist.at<float>(i-1))),
                          color, CV_FILLED);
            }

            // draw value histogram
            for(int i=1; i < bins[2]; ++i)
            {
                Scalar color(0,0,255);
                rectangle(orig_hist_images[2],
                          Point((i-1)*v_bin_w, H),
                          Point(i*v_bin_w, H - cvRound(H*v_hist.at<float>(i-1))),
                          color, CV_FILLED);
            }

            showImages();
        }

        void showImages()
        {
            namedWindow("h_hist", 0);
            namedWindow("s_hist", 0);
            namedWindow("v_hist", 0);
            imshow("h_hist", orig_hist_images[0]);
            imshow("s_hist", orig_hist_images[1]);
            imshow("v_hist", orig_hist_images[2]);
        }

        void drawLines()
        {
            //copy originals
            orig_hist_images.at(0).copyTo(h_hist_image);
            orig_hist_images.at(1).copyTo(s_hist_image);
            orig_hist_images.at(2).copyTo(v_hist_image);

            line(h_hist_image, Point( W * ledTracker->lowH / 179.0, 0), Point(W * ledTracker->lowH / 179.0, H-1), Scalar(255,255,0), 1);
            line(h_hist_image, Point( W * ledTracker->highH / 179.0, 0), Point(W * ledTracker->highH / 179.0, H-1), Scalar(255,255,0), 1);
            line(h_hist_image, Point( W * h_median / 179.0, 0), Point(W * h_median / 179.0, H-1), Scalar(0,255,255), 1);
            line(h_hist_image, Point( W * h_median / 179.0, 0), Point(W * h_median / 179.0, H-1), Scalar(0,255,255), 1);

            line(s_hist_image, Point( W * ledTracker->lowS / 255.0, 0), Point(W * ledTracker->lowS / 255.0, H-1), Scalar(255,255,0), 1);
            line(s_hist_image, Point( W * ledTracker->highS / 255.0, 0), Point(W * ledTracker->highS / 255.0, H-1), Scalar(255,255,0), 1);
            line(s_hist_image, Point( W * s_median / 179.0, 0), Point(W * s_median / 179.0, H-1), Scalar(0,255,255), 1);
            line(s_hist_image, Point( W * s_median / 179.0, 0), Point(W * s_median / 179.0, H-1), Scalar(0,255,255), 1);

            line(v_hist_image, Point( W * ledTracker->lowV / 255.0, 0), Point(W * ledTracker->lowV / 255.0, H-1), Scalar(255,255,0), 1);
            line(v_hist_image, Point( W * ledTracker->highV / 255.0, 0), Point(W * ledTracker->highV / 255.0, H-1), Scalar(255,255,0), 1);
            line(v_hist_image, Point( W * v_median / 179.0, 0), Point(W * v_median / 179.0, H-1), Scalar(0,255,255), 1);
            line(v_hist_image, Point( W * v_median / 179.0, 0), Point(W * v_median / 179.0, H-1), Scalar(0,255,255), 1);

            // draw copies
            namedWindow("h_hist", 0);
            namedWindow("s_hist", 0);
            namedWindow("v_hist", 0);
            imshow("h_hist", h_hist_image);
            imshow("s_hist", s_hist_image);
            imshow("v_hist", v_hist_image);
        }

        void registerOrigHistImages(Mat h_hist_image, Mat s_hist_image, Mat v_hist_image)
        {
            Mat buffer;
            h_hist_image.copyTo(buffer);
            orig_hist_images.push_back(buffer);
            s_hist_image.copyTo(buffer);
            orig_hist_images.push_back(buffer);
            v_hist_image.copyTo(buffer);
            orig_hist_images.push_back(buffer);
        }

        Mat estimateLedByThreshold()
        {
            Mat im;
            Mat result = Mat::zeros(ledTracker->thresholded.rows, ledTracker->thresholded.cols, CV_8UC3);
            ledTracker->thresholded.copyTo(im);
            Contours contours;
            Contours contours_result;
            Contours contours_skipped;

            ledTracker->findSortedContours(im, contours);
            long nbContours = contours.size();
            if(nbContours > 0)
            {
                std::for_each(contours.begin(), contours.end(), [&](const Contour c)
                {
                    bool ok = false;
                    double area = boundingRect(c).area();
                    double whiteArea = ledTracker->getWhiteAreaOnRoi(boundingRect(c));
                    ok = ((minArea - minArea*alpha_area) <= area && area <= (maxArea + maxArea*alpha_area));
//                   ok &= minWhiteArea <= whiteArea && whiteArea <= maxWhiteArea;
                    if(ok)
                    {
                        contours_result.push_back(c);
//                       cout << "OK : " << area <<   " in [" << minArea<< ":" << maxArea << "]" << endl;
                    }
                    else
                    {
//                       cout << "rejected: " << area <<   " not in [" << minArea<< ":" << maxArea << "]" << endl;
                        contours_skipped.push_back(c);
                    }
                });

                cout << endl;
                drawContours(result, contours_result, -1, Scalar(0,255,0), CV_FILLED);
                drawContours(result, contours_skipped, -1, Scalar(0,0,255), CV_FILLED);

            }

            if(DEBUG_MAIN)
            {
                namedWindow("result", 0);
                imshow("result", result);
            }

            vector<Mat> results;
            split(result, results);
            return results[1];
        }
    };
    ModelHistogram ledModel;

public:
    typedef vector<Point> Contour;
    typedef vector<Contour> Contours;

    //////////////// methodes statics ///////////////////

    static LedTracker* getInstance()
    {
        if(instance == nullptr)
            instance = new LedTracker();
        return instance;
    }

    static void onThresholdChanged(int, void*)
    {
        LedTracker::getInstance()->processThreshold();
    }

    /////////////// methode non-statics //////////////////
    void setFrame(Mat frame)
    {
        image = frame;
        image.copyTo(original);
        cvtColor(image, hsv, CV_BGR2HSV);
    }

    void activateDebugAnalysis()
    {
        if(DEBUG)
        {
            namedWindow("threshold", 0);
            createTrackbar("lowH", "threshold", &lowH, 179, onThresholdChanged);
            createTrackbar("lowS", "threshold", &lowS, 255, onThresholdChanged);
            createTrackbar("lowV", "threshold", &lowV, 255, onThresholdChanged);
            createTrackbar("highH", "threshold", &highH, 179, onThresholdChanged);
            createTrackbar("highS", "threshold", &highS, 255, onThresholdChanged);
            createTrackbar("highV", "threshold", &highV, 255, onThresholdChanged);
        }
    }

    void threshold_circular(Mat* hsv, Scalar low, Scalar high, Mat* thresholded)
    {
        // Hue range is circular [0:179] deg
        if(low[0] <= high[0])
        {
            // min Hue value <= max value (i.e [20:160])
            inRange(*hsv, low, high, *thresholded);
        }
        else
        {
            // min Hue value >= max value (i.e: [160:20])
            vector<Mat> hsv_planes;
            split(*hsv,hsv_planes);

            Mat maskH, maskS, maskV;
            inRange(hsv_planes[0], high[0], high[0], maskH);    // inRange Hue (max -> min)
            inRange(hsv_planes[1], low[1], high[1], maskS);    // inRange Sat (normal)
            inRange(hsv_planes[2], low[2], high[2], maskV);    // inRange Val (normal)

            bitwise_and(maskS, maskV, *thresholded);        // merge Sat & Val
            bitwise_not(maskH, maskH);                     // invese Hue range
            bitwise_and(maskH, *thresholded, *thresholded);  // merge All
        }
    }

    void processThreshold()
    {

        // Hue range is circular [0:179] deg
        if(lowH <= highH)
        {
            // min Hue value <= max value (i.e [20:160])
            inRange(hsv, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), thresholded);
        }
        else
        {
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

        Mat masked;
        image.copyTo(masked, thresholded);

        if(DEBUG)
        {
            namedWindow("threshold",0);
            imshow("threshold", masked);
            ledModel.drawLines();
        }
    }

    Point findObjectPosition(TRACKING_METHOD method)
    {
        processThreshold();

        Point pos(-1,-1);
        switch(method)
        {
        case FAST:
            if(!thresholded.empty())
            {
                Mat filterByArea = ledModel.estimateLedByThreshold();
                pos = FindLightCenter(filterByArea);
                if(DEBUG_MAIN && pos.x > 0 && pos.y > 0)
                {
                    drawCross(image, pos, Scalar(0,0,255), 20, 3);
                }
            }
            break;
        case PRECISION:
            break;
        }
        return pos;
    }

    void getPointfilterByArea()
    {
        if(!thresholded.empty())
        {
        }
    }

    Point FindLightCenter(Mat img)
    {
        Point p(-1,-1);
        Contours cts;
        findSortedContours(img, cts);
        if(cts.size() > 0)
        {
            Moments oMoments = moments(cts.at(0));
            double m01 = oMoments.m01;
            double m10 = oMoments.m10;
            double mArea = oMoments.m00;
            p = Point(m10/mArea,m01/mArea);
        }
        return p;
    }

    double getWhiteAreaOnRoi(Rect rect_roi)
    {
        if(rect_roi.area() == 0)
            return 0;
        Mat roi = hsv(rect_roi);
        Mat center_thresold;

        inRange(roi, Scalar(0,0,0), Scalar(0,0,255), center_thresold);

        if(DEBUG)
        {
            namedWindow("white", 0);
            imshow("white", center_thresold);
        }

        Contours cts;
        findSortedContours(center_thresold, cts);
        if(cts.size() == 0)
        {
            return 0;
        }

        Moments oMoments = moments(cts.at(0));
        double area = oMoments.m00;
        return area;
        return 0;
    }

    double getWhiteAreaOnRoi()
    {
        return getWhiteAreaOnRoi(rect_mask_roi);
    }

    Point FindLedCenter(Mat img)
    {
        Point p(-1,-1);
        Mat roi;
        Mat center_thresold;

        Rect rect = getMaxBoundingRect(img);
        if(rect.area() == 0)
        {
            return p;
        }
        roi = img(rect);
        inRange(roi, Scalar(0,0,0), Scalar(0,0,255), center_thresold);
        Contours cts;

        findSortedContours(center_thresold, cts);
        if(cts.size() > 0)
        {
            Moments oMoments = moments(cts.at(0));
            double m01 = oMoments.m01;
            double m10 = oMoments.m10;
            double mArea = oMoments.m00;
            p = Point(m10/mArea,m01/mArea);
            p.x += rect.x;
            p.y += rect.y;
        }

        if(DEBUG)
        {
            namedWindow("ledCenter", 0);
            imshow("ledCenter", center_thresold);
        }
        return p;
    }

    void drawCross(Mat &frame, Point center, Scalar color = Scalar(0,255,255), int size = 5, int thickness = 1)
    {
        line(frame, Point(center.x, center.y -size), Point(center.x,center.y + size ), color,thickness);
        line(frame, Point(center.x-size, center.y), Point(center.x + size,center.y), color, thickness);
    }

    void processLedAnalysis()
    {

        if(isModelReady())
        {
            current_mode = TRACKING;
        }

        switch (current_mode)
        {
        case NONE:
            if(!processForeground())
                break;                  // foreground empty nothing todo
            rect_mask_roi.area();
            ledModel.calcHistograms(mask_roi, mask_hist);
            processThreshold();
            showMaskRoi();
            break;
        case CALIBRATION:
            calibrate();
            break;
        case TRACKING:
            processThreshold();
            this->findObjectPosition(tracking_method);
            break;
        default:
            break;
        }

        showMainImages();
    }

    void calibrate()
    {
        if(!processForeground())
            return;                 // foreground empty nothing todo
        ledModel.calcHistograms(mask_roi, mask_hist, true);
        showMaskRoi();
    }

    int getNbCalibrationSteps()
    {
        return ledModel.getNbSampleRequired();
    }

    int getCurrentCalibrationStep()
    {
        return ledModel.getNbSampleInModel();
    }

    bool isModelReady()
    {
        return getNbCalibrationSteps() == getCurrentCalibrationStep();
    }

    bool processForeground()
    {

        backgroundSubtractor->apply(image, foreground, learningRate);
        removeNoise(foreground);

        if(object_type == STYLUS)
        {
            Mat mask_people;
//            threshold_circular(&hsv, default_stylus_params.low_mask_people,  default_stylus_params.high_mask_people, &mask_people);
//            bitwise_and(mask_people, foreground, foreground);

            // Hue range is circular [0:179] deg
            if(default_stylus_params.low_mask_people[0] <= default_stylus_params.high_mask_people[0])
            {
                // min Hue value <= max value (i.e [20:160])
//                cout << "inRange: " << default_stylus_params.low_mask_people << " : " << default_stylus_params.high_mask_people << endl;
                inRange(hsv, default_stylus_params.low_mask_people, default_stylus_params.high_mask_people, mask_people);
            }
            else
            {
                // min Hue value >= max value (i.e: [160:20])
                vector<Mat> hsv_planes;
                split(hsv,hsv_planes);

                Mat maskH, maskS, maskV;
                inRange(hsv_planes[0], default_stylus_params.high_mask_people[0], default_stylus_params.low_mask_people[0], maskH);    // inRange Hue (max -> min)
                inRange(hsv_planes[1], default_stylus_params.low_mask_people[1], default_stylus_params.high_mask_people[1], maskS);    // inRange Sat (normal)
                inRange(hsv_planes[2], default_stylus_params.low_mask_people[2], default_stylus_params.high_mask_people[2], maskV);    // inRange Val (normal)

                bitwise_and(maskS, maskV, mask_people);        // merge Sat & Val
                bitwise_not(maskH, maskH);                     // invese Hue range
                bitwise_and(maskH, mask_people, mask_people);  // merge All
            }

            erode(mask_people, mask_people, getStructuringElement(MORPH_ELLIPSE, Size(10,10)));
            dilate(mask_people, mask_people, getStructuringElement(MORPH_ELLIPSE, Size(10,10)));
            dilate(mask_people, mask_people, getStructuringElement(MORPH_ELLIPSE, Size(10,10)));
            erode(mask_people, mask_people, getStructuringElement(MORPH_ELLIPSE, Size(10,10)));

            bitwise_and(mask_people, foreground, foreground);

            if(DEBUG)
            {
                Mat masked;
                image.copyTo(masked, mask_people);
                namedWindow("mask_people", 0);
                imshow("mask_people", masked);
            }
        }

        // calculate bouding rect for moving foreground
        rect_mask_roi = getMaxBoundingRect(foreground);
        if(rect_mask_roi.area() == 0) return false;

        // mask foreground and isolate in region of interset window
        hsv.copyTo(mask_roi, foreground);
        mask_roi = mask_roi(rect_mask_roi);
        mask_hist = foreground(rect_mask_roi);
        return true;
    }

    void showMaskRoi(bool rgb = true)
    {
        if(DEBUG)
        {
            namedWindow("roi", 0);
            if(rgb)
            {
                Mat rgb_image;
                cvtColor(mask_roi, rgb_image, CV_HSV2BGR);
                imshow("roi", rgb_image);
            }
            else
            {
                imshow("roi", mask_roi);
            }
        }
    }

    void removeNoise(Mat im, bool deep = true)
    {
        erode(im, im, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(im, im, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        if(!deep) return;
        dilate(im, im, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(im, im, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    }

    Rect getMaxBoundingRect(Mat mask)
    {
        Contours contours;
        Rect rect;

        // find contours
        findSortedContours(mask, contours);
        if(contours.size() > 0)
        {
            // get bounding rect of the max area contour
            rect = boundingRect(contours.at(0));
        }

        return rect;
    }

    void findSortedContours(Mat image, Contours& contours)
    {
        Mat im;
        image.copyTo(im);
        findContours(im, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
        if(contours.size() > 0)
        {
            // sort contours by area
            sort(contours.begin(), contours.end(), [&](const Contour c1, const Contour c2)
            {
                return contourArea(c1) > contourArea(c2);
            });
        }
    }

    void showMainImages()
    {
        if(DEBUG_MAIN)
        {
            namedWindow("original",0);
            namedWindow("foreground",0);
            imshow("original", image);
            imshow("foreground", foreground);
        }
    }

    void setMode(LedTracker::MODE mode)
    {
        current_mode = mode;
    }

    void setTrackingMethod(TRACKING_METHOD method)
    {
        tracking_method = method;
    }

    void setObjectType(OBJECT_TYPE type)
    {
        object_type = type;
    }

    LedTracker(): ledModel(this)
    {
        backgroundSubtractor = createBackgroundSubtractorMOG2(500, 60, false);
        default_stylus_params.low_mask_people = Scalar(134,0,0);            // 134,89,108
        default_stylus_params.high_mask_people = Scalar(179,255,255);       // 10,255,255
        default_stylus_params.lowH = 134;
        default_stylus_params.highH = 10;
    }

    ~LedTracker()
    {
    }
};

#endif // LEDTRACKER

