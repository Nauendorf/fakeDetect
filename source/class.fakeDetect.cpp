#include "fakeDetect.h"

class Configuration
{
    public:
    int debug;
    int blur_uv_ksize;
    int blur_ir_ksize;
    float frame_percent;
    std::string confPath = "./values.conf";
    
    Configuration()
    {
        std::ifstream s;
        std::string line;
        s.open(Configuration::confPath);
        std::map<std::string, std::string> m;        

        if (s.is_open())
        {
            while (std::getline(s, line))
            {
                int num = line.find("=");
                std::string key;
                std::string value;

                if (num != -1)
                {
                    key = line.substr(0, num);
                    value = line.substr(num + 1, line.length());
                    m[key] = value;
                }      
            }
            s.close();
        }

        this->debug = atoi(m["debug"].c_str());
        this->blur_uv_ksize = atoi(m["blur_uv_ksize"].c_str());
        this->blur_ir_ksize = atoi(m["blur_ir_ksize"].c_str());
        this->frame_percent = atof(m["frame_percent"].c_str());
    }
};

class Passport
{
    public:
    Passport(){}
    ~Passport(){}

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

    bool pixelcountCheck(cv::Mat Img)
    {
        int countBlack = cv::countNonZero(Img);
        float percent = (countBlack * 100.00) / (Img.cols * Img.rows);
        //TODO: return metrics for adding to metadata output
        if (percent <= 0.5 || percent >= 15)
            return false;
        else
            return true;
    }

    bool coordCheck(cv::Point coord)
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

    float getUvPercentage(const cv::Mat& mat)
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

    float getIrBrightness(cv::Mat irImage)
    {
        cv::Mat hsv;
        cv::cvtColor(irImage, hsv, CV_BGR2HSV);
        float irResult = cv::mean(hsv)[2];

        // cv::mean() will return 3 numbers, one for each channel:
        // 0=hue;     1=saturation;    2=value (brightness)

        return irResult;
    }

    std::vector<cv::Rect> getFieldSegmentCoords(const cv::Rect& field, int segmentCount)
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

};