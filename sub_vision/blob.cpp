#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    
    // Read image
    Mat im;
    Mat filtered;
    Mat mask;
    VideoCapture cap("DSCF0678_Trim.mp4");
    if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    while(1){
        cap>>im;
        if(im.empty())
            break;
        cv::cvtColor(im, im, COLOR_BGR2GRAY);
        // Setup SimpleBlobDetector parameters.
        SimpleBlobDetector::Params params;
        
        // Change thresholds
        params.minThreshold = 70;
        params.maxThreshold = 110;
        
        //Filter by Color (does it work)?
        params.filterByColor = false;
        params.blobColor =  100;
        
        // Filter by Area.
        params.filterByArea = true;
        params.minArea = 50;
        
        // Filter by Circularity
        params.filterByCircularity = false;
        params.minCircularity = 0.785;//this is the circularity of a square
        
        // Filter by Convexity
        params.filterByConvexity = false;
        params.minConvexity = 0.87;
        
        // Filter by Inertia
        params.filterByInertia = false;
        params.minInertiaRatio = 0.01;
        
        
        // Storage for blobs
        vector<KeyPoint> keypoints;
        
        
#if CV_MAJOR_VERSION < 3   // If you are using OpenCV 2
        
        // Set up detector with params
        SimpleBlobDetector detector(params);
        
        // Detect blobs
        detector.detect( im, keypoints);
#else
        
        // Set up detector with params
        Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
        
        // Detect blobs
        detector->detect( im, keypoints);
#endif
        
        // Draw detected blobs as red circles.
        // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
        // the size of the circle corresponds to the size of blob
        
        Mat im_with_keypoints;
        drawKeypoints( im, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        
        // Show blobs
        imshow("keypoints", im_with_keypoints );
        imshow("mask",im<80);
        // extract the x y coordinates of the keypoints:
        //
        //        for (int i=0; i<keypoints.size(); i++){
        //            float X = keypoints[i].pt.x;
        //            float Y = keypoints[i].pt.y;
        //        }
        
        waitKey(50);
    }
    
}
