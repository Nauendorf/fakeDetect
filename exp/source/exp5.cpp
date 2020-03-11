// Taken from: https://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html
// https://docs.opencv.org/3.4/df/d0d/tutorial_find_contours.html
// https://github.com/opencv/opencv/tree/3.4/samples/cpp/tutorial_code/ShapeDescriptors

// Compile: g++ -g example.cpp -o FakeIDDetector `pkg-config --cflags --libs opencv` -std=c++17

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat src_gray;
int thresh = 100;

/// Function header
void thresh_callback(int, void* );

int main( int argc, char* argv[] )
{
	Mat src = imread(argv[1]);
    Mat img; Mat templ; Mat result;
    Mat compareTo = imread(argv[1]);
    int match_method;
    string image_window = "Source Image";
    string result_window = "Result window";

	if( src.empty() )
	{
		cout << "Could not open or find the image!\n" << endl;
		cout << "Usage: " << argv[0] << " <Input image>" << endl;
		return -1;
	}
	
	/// Convert image to gray and blur it
	cvtColor( src, src_gray, COLOR_BGR2GRAY );
	blur( src_gray, src_gray, Size(3,3) );

	// FIDDLE: the 60 number (lower more, higher less)
	threshold(src, templ, 70, 255 , CV_THRESH_BINARY);

	resize(templ, templ, cv::Size(src.cols/2,src.rows/2));

	/// Create Window
	const char* source_window = "Source";
	namedWindow( source_window );
	imshow( source_window, templ );
    /// Create windows
    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );

	const int max_thresh = 255;

    /// Detect edges using Canny
    Mat canny_output;
    Canny( src_gray, canny_output, thresh, thresh*2 );

    /// Find contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );

    /// Draw contours
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( 255, 0, 0 );
        drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
    }

    resize(drawing, drawing, cv::Size(drawing.cols/2,drawing.rows/2));
    /// Show in a window
    //imshow( "Contours", drawing );
	waitKey();


    /// Do the Matching and Normalize
    //matchTemplate( compareTo, templ, result, cv::TM_CCOEFF );
    //normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    /// Localizing the best match with minMaxLoc
    //double minVal; double maxVal; Point minLoc; Point maxLoc;
    //Point matchLoc;
//
    //minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
//
    ///// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    //if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    //{ matchLoc = minLoc; }
    //else
    //{ matchLoc = maxLoc; }
//
    ///// Show me what you got
    //rectangle( src, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
    //rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
//
    //imshow( image_window, src );
    //imshow( result_window, result );



	return 0;
}



