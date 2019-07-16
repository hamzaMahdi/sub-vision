//
//  hough.cpp
//  sub_vision
//
//  Created by Hamza Mahdi on 2019-07-16.
//  Copyright © 2019 Hamza Mahdi. All rights reserved.
//
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
        Mat dst, cdst;
        Canny(im, dst, 50, 200, 3);
        cvtColor(dst, cdst, COLOR_GRAY2BGR);
        
#if 0
        vector<Vec2f> lines;
        HoughLines(dst, lines, 1, CV_PI/180, 10, 0, 0 );
        
        for( size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
        }
#else
        vector<Vec4i> lines;
        HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
        for( size_t i = 0; i < lines.size(); i++ )
        {
            Vec4i l = lines[i];
            line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3 );
        }
#endif
        imshow("source", im);
        imshow("detected lines", cdst);
        
        waitKey(50);
    }
    
}
