#include "Passport.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
Passport::Passport()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
Passport::~Passport()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
cv::Rect Passport::getFrame(cv::MatSize imageSize, float fp)
{
	int height = imageSize[0];
	int width = imageSize[1];
	float padTop = height * fp;
	float padLeft = width * fp;
	float newHeight = height - (padTop * 2);
	float newWidth = width - (padLeft * 2);
	cv::Rect frameRect(padTop, padLeft, newWidth, newHeight);
	return frameRect;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Passport::pixelcountCheck(cv::Mat Img)
{
	int countBlack = cv::countNonZero(Img);
	float percent = (countBlack * 100.00) / (Img.cols * Img.rows);
	//TODO: return metrics for adding to metadata output
	if (percent <= 0.5 || percent >= 15)
		return false;
	else
		return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Passport::coordCheck(cv::Point coord)
{
	//TODO: Move to config not hard code once generalised 
	//TODO: return metrics for adding to metadata output
	if ((coord.x < 250) || (coord.x > 600))
		return false;
	if ((coord.y < 200) || (coord.y > 650))
		return false;
	else
		return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Passport::checkUV(cv::Mat Img)
{
	//Not a great check, but already in place.
	if (Img.rows > 500) {
		int H, W, uvw, uvh;

		//Crop 100px per side
		//UV box 200px^2
		//TODO: These can be calculated based on resolution later
		int os = 100;
		int ros = 200;

		//Convert to grey and measure image for crop
		cv::Mat gray;
		cv::cvtColor(Img, gray, cv::COLOR_BGR2GRAY);

		H = Img.rows;
		W = Img.cols;

		//Remove edged due to blow out on cuts
		cv::Rect uvROI;
		uvROI.x = os;
		uvROI.y = os;
		uvROI.width = H-os;
		uvROI.height = H-os;
		cv::Mat crop = Img(uvROI);

		//Large window median blur for removal of small bright areas
		cv::Mat blur;
		cv::medianBlur(crop, blur, 31);

		//get min max vals and stretch max_loc into square to include whole UV Feature (ros and os should be in training)
		//Using x here so that a negative value doesn't wreck opencv
		double min_val;
		double max_val;
		cv::Point min_loc(0,0);
		cv::Point max_loc(0,0);

		cv::cvtColor(blur, blur, cv::COLOR_BGR2GRAY);
		cv::minMaxLoc(blur, &min_val, &max_val, &min_loc, &max_loc);
		cv::Rect uvsq;
		uvsq.x = cv::max(0,(max_loc.x-ros));
		uvsq.y = cv::max(0,(max_loc.y-ros));
		uvsq.width = 400;
		uvsq.height = 400;

		//crop just the UVF
		cv::Mat UVF = crop(cv::Rect(uvsq.x,uvsq.y,uvsq.width,uvsq.height));

		cv::cvtColor(UVF,UVF,CV_BGR2GRAY);
		//get calculated global threshold for localised UV feature
		double th = cv::threshold(UVF, UVF, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

		//Normalise for min_val. If this isn't done can lead to blowout
		th = th + min_val; 

		//Static threshold on whole image using uv localised value
		cv::threshold(gray, gray, th, 255, CV_THRESH_BINARY);

		//Call pixel and coord checks
		bool pc = pixelcountCheck(gray);
		bool cc = coordCheck(max_loc);

		if (pc && cc)
		{
			return false;
		}
		else 
		{
			return true;
		}
	} else {
		return false;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
float Passport::getUvPercentage(const cv::Mat& mat)
{
	cv::Mat hsv, thrsd;
	cvtColor(mat, hsv, CV_BGR2HSV);
	cv::Mat channels[3];
	split(hsv, channels);
	cv::Mat selChannel = channels[2];
	selChannel = selChannel > 50;
	int countBlack = countNonZero(selChannel);
	return (countBlack * 100.00) / (mat.cols * mat.rows);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
float Passport::getIrBrightness(cv::Mat irImage)
{
	cv::Mat hsv;
	cv::cvtColor(irImage, hsv, CV_BGR2HSV);
	float irResult = cv::mean(hsv)[2];

	// cv::mean() will return 3 numbers, one for each channel:
	// 0=hue;     1=saturation;    2=value (brightness)

	return irResult;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<cv::Rect> Passport::getFieldSegmentCoords(const cv::Rect& field, int segmentCount)
{
	std::vector<cv::Rect>ResultArray;
	float segmentWidth = field.width / segmentCount; 

	for (int i=0; i<=segmentCount; i++)
	{
		if (i == 0)
		{
			float segPad = field.x;
			cv::Rect segRect(segPad, field.y, segmentWidth, field.height);
			ResultArray.push_back(segRect);			
		}

		float segPad = field.x + (segmentWidth * i);
		cv::Rect segRect(segPad, field.y, segmentWidth, field.height);
		ResultArray.push_back(segRect);
	}

	return ResultArray;
}