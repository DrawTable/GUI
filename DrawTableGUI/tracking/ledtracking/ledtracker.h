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

class LedTracker{

public:
     enum MODE {NONE, CALIBRATION, TRACKING};
     enum OBJECT_TYPE {POINTER, STYLUS};
     enum TRACKING_METHOD {FAST, PRECISION};
private:

    MODE current_mode = MODE::NONE;
    OBJECT_TYPE object_type = OBJECT_TYPE::STYLUS;
    TRACKING_METHOD tracking_method = FAST;

    // default params for Stylus mode
    struct Params{
        int lowH;
        int highH;
        Scalar low_mask_people;
        Scalar high_mask_people;
    } default_stylus_params;

    static LedTracker* instance;

    Mat original, image, hsv, thresholded;

    // region of interset (LED) of the original image
    Mat mask_roi, mask_hist;
    Rect rect_mask_roi;

    int lowH = 0, lowS = 0, lowV = 0, highH = 179, highS = 255, highV = 255;

    // Keep only moving objects
    Ptr<BackgroundSubtractorMOG2> backgroundSubtractor;
    double learningRate = 0.7;
    Mat foreground;

    // modeles probabilistes de la LED à détecter
    class ModelHistogram {

        LedTracker* ledTracker;

        // models histograms for H,S and V
        MatND h_hist_model,
              s_hist_model,
              v_hist_model;

        unsigned long nbSampleInModel = 0;
        const int nbSampleRequired = 50;

        MatND h_hist_buffer,
              s_hist_buffer,
              v_hist_buffer;

        // image representation of histograms
        Mat h_hist_image,
            s_hist_image,
            v_hist_image;

        vector<Mat> orig_hist_images;

        /////////////// Parameters for cv::calsHist ///////////////////////
        ///
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

        /////////// Parameters for image representations of hist //////////

        const int W, H;
        int h_bin_w;
        int s_bin_w;
        int v_bin_w;

        /////////////// Led size estimation ranges ///////////////////////

        Mat estimatedLed;
        int minArea = DEFAULT_MAX_AREA;
        int maxArea = 0;
        int minWhiteArea = DEFAULT_MAX_AREA;
        int maxWhiteArea = 0;
        const int alpha_area = 0.9;

    public:
        ModelHistogram(LedTracker* parent);
        int getNbSampleRequired();
        int getNbSampleInModel();
        void calcHistograms(Mat hsv_img, Mat mask = Mat(), bool cumulate_in_model = false);
        void processHistogramAnalysis(Mat *h_hist, Mat *s_hist, Mat *v_hist,
                                      Mat *h_hist_cumul, Mat *s_hist_cumul, Mat *v_hist_cumul, Mat* hsv_img = nullptr, Mat* mask = nullptr );
        void normalizeSum(Mat *hist, Mat *dest, Vec2i &bin_ranges, Vec2f probability_min, Range thres = Range(0,0));
        void showModelHist();
        void showHistograms(Mat h_hist, Mat s_hist, Mat v_hist);
        void showImages();
        void drawLines();
        void registerOrigHistImages(Mat h_hist_image, Mat s_hist_image, Mat v_hist_image);
        Mat estimateLedByThreshold();
    };
    ModelHistogram ledModel;

public:
    LedTracker();
    typedef vector<Point> Contour;
    typedef vector<Contour> Contours;

    static LedTracker *getInstance();
    static void onThresholdChanged(int, void*);

    void setFrame(Mat frame);
    void activateDebugAnalysis();
    void threshold_circular(Mat* hsv, Scalar low, Scalar high, Mat* thresholded);
    void processThreshold();
    Point findObjectPosition(TRACKING_METHOD method);
    Point FindLightCenter(Mat img);
    double getWhiteAreaOnRoi(Rect rect_roi);
    double getWhiteAreaOnRoi();
    Point FindLedCenter(Mat img);
    void drawCross(Mat &frame, Point center, Scalar color = Scalar(0,255,255), int size = 5, int thickness = 1);
    void processLedAnalysis();
    void calibrate();
    int getNbCalibrationSteps();
    int getCurrentCalibrationStep();
    bool isModelReady();
    bool processForeground();
    void showMaskRoi(bool rgb = true);
    void removeNoise(Mat im, bool deep = true);
    Rect getMaxBoundingRect(Mat mask);
    void findSortedContours(Mat image, Contours& contours);
    void showMainImages();
    void setMode(LedTracker::MODE mode);
    void setTrackingMethod(TRACKING_METHOD method);
    void setObjectType(OBJECT_TYPE type);
    ~LedTracker();
};

#endif // LEDTRACKER

