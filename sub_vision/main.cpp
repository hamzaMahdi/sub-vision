#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>

using namespace std;
using namespace cv;

/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2
 */
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/**
 * Helper function to display text in the center of a contour
 */
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.4;
    int thickness = 1;
    int baseline = 0;
    
    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);
    
    cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), FILLED);
    cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}
void find_moments(Mat);
int main()
{
    //cv::Mat src = cv::imread("polygon.png");
    cv::Mat src;
    cv::Mat gray;
    cv::Mat bw;
    cv::Mat dst;
    cv::Mat filtered;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Point> approx;
    
    VideoCapture cap("DSCF0678_Trim.mp4");
    if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    
    //    capture >> src;
    //src = imread("Reccc-0.png");
    while(1){
        Mat src;
        // Capture frame-by-frame
        cap >> src;
        
        // If the frame is empty, break immediately
        if (src.empty())
            break;
        
    //cv::imshow("test",src);
    
    if (true)
    {
        
        // Convert to grayscale
        cv::cvtColor(src, gray, COLOR_BGR2GRAY);
        
        // Use Canny instead of threshold to catch squares with gradient shading
        blur(gray, bw, Size(5, 5));
        // Smoothing without removing edges.
        bilateralFilter ( gray, filtered, 15, 80, 80 );
        cv::Canny(filtered, bw, 10, 300, 3); //80,240
        

        //cv::bitwise_not(bw, bw);
        std::cout<<"--------";
        //find_moments(bw);
        // Find contours
        /*cv::findContours(bw.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        
        src.copyTo(dst);
        
        for (int i = 0; i < contours.size(); i++)
        {
            // Approximate contour with accuracy proportional
            // to the contour perimeter
            cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
            
            // Skip small or non-convex objects
            if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
                continue;
            
            if (approx.size() == 3)
            {
                setLabel(dst, "TRI", contours[i]);    // Triangles
            }
            else if (approx.size() >= 4 && approx.size() <= 6)
            {
                // Number of vertices of polygonal curve
                int vtc = approx.size();
                
                // Get the cosines of all corners
                std::vector<double> cos;
                for (int j = 2; j < vtc + 1; j++)
                    cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));
                
                // Sort ascending the cosine values
                std::sort(cos.begin(), cos.end());
                
                // Get the lowest and the highest cosine
                double mincos = cos.front();
                double maxcos = cos.back();
                
                // Use the degrees obtained above and the number of vertices
                // to determine the shape of the contour
                if (vtc == 4)
                    setLabel(dst, "RECT", contours[i]);
                else if (vtc == 5)
                    setLabel(dst, "PENTA", contours[i]);
                else if (vtc == 6)
                    setLabel(dst, "HEXA", contours[i]);
                else if (vtc == 8)
                    setLabel(dst, "OCT", contours[i]);
            }
            else
            {
                // Detect and label circles
                double area = cv::contourArea(contours[i]);
                cv::Rect r = cv::boundingRect(contours[i]);
                int radius = r.width / 2;
                
                if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
                    std::abs(1 - (area / (CV_PI * (radius*radius)))) <= 0.2)
                    setLabel(dst, "CIR", contours[i]);
            }
        }
         */
        cv::imshow("src", filtered);
        cv::imshow("dst", gray);
        cv::imshow("bw", bw);
        waitKey(100);
        
    }
    }
    
    waitKey(0);
    
    
    return 0;
}



void find_moments(Mat gray)
{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    /// Detect edges using canny
    Canny(gray, canny_output, 50, 150, 3);
    // Find contours
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    
    /// Get the moments
    vector<Moments> mu(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        mu[i] = moments(contours[i], false);
    }
    
    ///  Get the centroid of figures.
    vector<Point2f> mc(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
        cout << mc[i];
    }
    
    
    /// Draw contours
    
    Mat drawing(canny_output.size(), CV_8UC3, Scalar(255, 255, 255));
    
    for (int i = 0; i< contours.size(); i++)
    {
        Scalar color = Scalar(167, 151, 0);
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        circle(drawing, mc[i], 4, color, -1, 7, 0);
    }
    
    /// Show the resultant image
    namedWindow("Contours", WINDOW_AUTOSIZE);
    imshow("Contours", drawing);
    //waitKey(0);
     
    
}
