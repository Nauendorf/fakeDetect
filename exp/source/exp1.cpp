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

int main( int argc, char** argv )
{
	Mat src = imread("HappyID.jpg");
	Mat test = src.clone();

	if( src.empty() )
	{
		cout << "Could not open or find the image!\n" << endl;
		cout << "Usage: " << argv[0] << " <Input image>" << endl;
		return -1;
	}
	
	/// Convert image to gray and blur it
	cvtColor( src, src_gray, COLOR_BGR2GRAY );
	blur( src_gray, src_gray, Size(3,3) );
	threshold(src, src, 100, 255 , CV_THRESH_BINARY);

	/// Create Window
	const char* source_window = "Source";
	namedWindow( source_window );
	imshow( source_window, src );

	const int max_thresh = 255;
	createTrackbar( "Canny thresh:", source_window, &thresh, max_thresh, thresh_callback );
	thresh_callback( 0, 0 );

	waitKey();
	return 0;
}

void thresh_callback(int, void* )
{
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

	/// Show in a window
	//imshow( "Contours", drawing );
}