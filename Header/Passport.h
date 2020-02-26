#include "fakeDetect.h"

class Passport
{
    public:
								Passport				();
		virtual					~Passport				();

    	cv::Rect 				getFrame				(cv::MatSize imageSize, float fp);
		bool 					pixelcountCheck			(cv::Mat Img);
   		bool 					coordCheck				(cv::Point coord);
    	bool 					checkUV					(cv::Mat Img);
    	float 					getUvPercentage			(const cv::Mat& mat);
    	float 					getIrBrightness			(cv::Mat irImage);
   		std::vector<cv::Rect> 	getFieldSegmentCoords	(const cv::Rect& field, int segmentCount);
};