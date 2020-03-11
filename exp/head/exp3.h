/*
    Author: David Nauendorf
    Date: March 02 2020
    
    Checks the Infrared layer of a scan for clusters of dark pixels to
    determine whether or not information exists. If the ir layer is blank
    then it is likely a fake or altered.
*/
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <map>

// Headers
void        showImage          (cv::Mat &img, std::string title, bool resized=1);
bool        irClusterCheck     (cv::Mat irImage, cv::Mat uvImage, int MaxClusters=180, float p=0.1);
cv::Rect    getFrame           (cv::MatSize imageSize, float p);