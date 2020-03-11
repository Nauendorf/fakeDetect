#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] )
{
    cv::Mat src = cv::imread(argv[1]);
    cv::Mat templ = cv::imread(argv[2]);
    cv::Mat dst;
    cv::Mat mask;
    cv::Point minLoc(0, 0);
    cv::Point maxLoc(0,0);
    double min_val;
    double max_val;

    cv::Mat cvt_src;
    //cv::matchTemplate(src, templ, dst, cv::TM_CCOEFF, mask);
    //cv::cvtColor(src, cvt_src, cv::COLOR_BGR2GRAY);
    cv::minMaxLoc(src, &min_val, &max_val, &minLoc, &maxLoc); 



    cv::Scalar color = cv::Scalar(255, 0, 0, 255);
    cv::Point point = cv::Point(maxLoc.x + templ.cols, maxLoc.y + templ.rows);
    cv::rectangle(src, maxLoc, point, color, 2, cv::LINE_8, 0);
    cv::imshow("canvasOutput", src);
    waitKey(0);
}