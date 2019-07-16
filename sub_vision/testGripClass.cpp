//
//  testGripClass.cpp
//
//
//  Created by Hamza Mahdi on 2019-07-16.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include "GripPipeline.h"
int main(int argc, char** argv) {
    //const char* file = argv[1];
    // Read image (color mode)
    cv::Mat img;
    img = cv::imread("rect.png");
    if(img.empty())
    {
        //cout << "Error in reading image" << file<< endl;
        return -1;
    }

    grip::GripPipeline processObject;
    processObject.Process(img);
    cv::imshow("test",*processObject.GetHsvThresholdOutput());
    
}
