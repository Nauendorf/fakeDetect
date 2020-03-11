#include "fakeDetect.h"

class Passport
{
    public:
    Passport(){}
    ~Passport(){}

    public:
    int debug;
    int blur_ir_ksize;
    float frame_percent;

    std::vector<cv::Point> getNeighbours(cv::Point p)
    {
        std::vector<cv::Point> pointArr;

        for (int i=0; i<25; i++)
        {
            float newx1 = (p.x - i); float newy1 = (p.y - i);
            float newx2 = (p.x + i); float newy2 = (p.y + i);
            float newx3 = (p.x + i); float newy3 = (p.y - i);
            float newx4 = (p.x - i); float newy4 = (p.y + i);

            cv::Point newPoint1 = cv::Point(newx1, newy1);
            cv::Point newPoint2 = cv::Point(newx2, newy2);
            cv::Point newPoint3 = cv::Point(newx3, newy3);
            cv::Point newPoint4 = cv::Point(newx4, newy4);

            pointArr.push_back(newPoint1);
            pointArr.push_back(newPoint2);
            pointArr.push_back(newPoint3);
            pointArr.push_back(newPoint4);
        }
        return pointArr;
    }

    cv::Mat findDarkPixels(cv::Mat &image, double min_val)
    {
        std::vector<cv::Point> pointArrj;
        std::vector<cv::Point> pointArri;
        std::vector<cv::Point> pointArr;

        for (int i=0; i<image.rows; i++)
        {
            for (int j=0; j<image.cols; j++)
            {
                cv::Point pixel_loc = cv::Point(j,i);
                cv::Scalar pixel_val = image.at<cv::Vec3b>(i,j);
                std::cout << "int at value  " << pixel_val << "\n";
                if (pixel_val[0] < min_val)
                {
                    cv::circle(image, pixel_loc, 50, cv::Scalar(66, 245, 215), 5);   
                }
            }
        }
        cv::Mat img;
        cv::resize(image, img, cv::Size(img.cols/2,img.rows/2));     
        cv::imshow("Naive", image);
        cv::waitKey(0);
        return image;
    }


    bool pixel_equal(const cv::Vec3b &a, const cv::Vec3b &b)
    {   return a == b;   } // this is the "exact match"
    // {   return norm(a,b) < 32;   } // if you want something more tolerant


    void showImage(cv::Mat &img, bool resized=0)
    {
        if (resized == 1)
        {
            cv::resize(img, img, cv::Size(img.cols/2,img.rows/2));
        }
        
        cv::imshow("Naive", img);
        cv::waitKey(0);
    }

    bool isInsideRect(cv::Rect roi, cv::Point point, cv::MatSize image_size)
    {
        int th = image_size[0];
        int tw = image_size[1];    
        float maxh = roi.y;
        float minh = (roi.height + roi.y);
        float maxw = roi.x + roi.width;
        float minw = roi.x;
        // std::cout << "th  " << th << "\n";
        // std::cout << "tw  " << tw << "\n";
        // std::cout << "maxh  " << maxh << "\n";
        // std::cout << "minh  " << minh << "\n";
        // std::cout << "maxw  " << maxw << "\n";
        // std::cout << "minw  " << minw << "\n";
        // std::cout << "point y  " << point.y << "\n";
        // std::cout << "point x  " << point.x << "\n";        

        if ( (point.y > minh || point.y < maxh) || (point.x < minw || point.x > maxw) ) 
        {
            // Out of bounds
            return false;
        }
        else if ( (point.y < minh || point.y > maxh) || (point.x > minw || point.x < maxw) )
        {
            // Within bounds
            return true;
        }

        return false;
    }

    // cv::MatSize imageSize || float fp (frame percentage)
    cv::Rect getFrame(cv::MatSize imageSize, float fp)
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

    float getIrBrightness(cv::Mat irImage)
    {
        cv::Mat hsv;
        cv::cvtColor(irImage, hsv, CV_BGR2HSV);
        float irResult = cv::mean(hsv)[2];

        // cv::mean() will return 3 numbers, one for each channel:
        // 0=hue;     1=saturation;    2=value (brightness)

        return irResult;
    }
};
