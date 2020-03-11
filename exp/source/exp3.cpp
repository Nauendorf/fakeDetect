#include "../head/exp3.h"

int main( int argc, char** argv )
{
    cv::Mat irImage = cv::imread(argv[1]);
    cv::Mat uvImage = cv::imread(argv[2]);
    //  180: Cluster count; 0.1: 10% reduction in frame
    bool clusters = irClusterCheck(irImage, uvImage, 120, 0.2);  

    if (clusters)
    {
        // ir layer has too few clusters and is probably blank
        std::cout << "irClusterCheck: true\n";
        std::cout << argv[1] << "\n";
        //showImage(irImage, argv[1], 1);
        //showImage(uvImage, argv[2], 1);
    }
    else
    {
        // We found enough clusters on ir to determine that information is visible as expected
        std::cout << argv[1] << "\n";
        std::cout << "irClusterCheck: false\n";
        //showImage(irImage, argv[1], 1);
        //showImage(uvImage, argv[2], 1);
    }
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

// Resize and display a cv::Mat with a given title
void showImage(cv::Mat &img, std::string title, bool resized)
{
    if (resized == 1)
    {
        cv::resize(img, img, cv::Size(img.cols/2,img.rows/2));
    }  
    cv::imshow(title, img);
    cv::waitKey(0);
}

// Cuts out a given percentage from the outside of the Mat (float p)
// Avoids the influence of shadows from page folds and the scanner plate when detecting clusters on the ir image
cv::Rect getFrame(cv::MatSize imageSize, float p)
{
    int height      = imageSize[0];
    int width       = imageSize[1];
    float padTop    = height * p;
    float padLeft   = width * p;
    float newHeight = height - (padTop * 2);
    float newWidth  = width - (padLeft * 2);
    cv::Rect frameRect(padTop, padLeft, newWidth, newHeight);
    return frameRect;
}  

float getUvPercentage(const cv::Mat& mat)
{
    const float uvThreshold = 95;
    cv::Mat hsv, thrsd;
    cv::cvtColor(mat, hsv, CV_BGR2HSV);
    cv::Mat channels[3];
    cv::split(hsv, channels);
    cv::Mat selChannel = channels[2];
    selChannel = selChannel > 50;
    int countBlack = countNonZero(selChannel);
    return (countBlack * 100.00) / (mat.cols * mat.rows);
}

// Counts clusters on the ir image and tests whether cluster count exceeds our MaxClusters threashold
// [ int MaxClusters ] Number of clusters that best distinguishes blank from legitimate ir layers
// [ float p ] defines frame size based on percentage smaller than image dimensions
bool irClusterCheck(cv::Mat irImage, cv::Mat uvImage, int MaxClusters, float p)
{
    if (irImage.rows > 500) 
    {
        cv::Mat framedIR;  cv::Mat framedThres;   cv::Mat m;  cv::Mat t;  std::map<int,int> clusters;
        cv::Rect frameIRRect = getFrame(irImage.size, p);
        cv::Rect frameThresRect = getFrame(irImage.size, 0.35);
        framedIR = irImage(frameIRRect);
        framedThres = irImage(frameThresRect);
        cv::medianBlur(framedIR, m, 31);
        cv::threshold(framedThres, t, 120, 255 , CV_THRESH_BINARY);
        float uvBrightness = getUvPercentage(uvImage);
        float irBrightness = getIrBrightness(t);

        for (size_t i=0; i<m.total(); i++)
        {
            cv::Vec3b v = m.at<cv::Vec3b>(i);
            int c = (v[0] << 16) | (v[1] << 8) | (v[2]);
            if (clusters.find(c) != clusters.end())
            {   clusters[c] ++;    }
            else
            {   clusters[c] = 0;   }
        }

        for (std::map<int,int>::iterator it=clusters.begin(); it != clusters.end(); )
        {
            if (it->second < 50)
            {   it = clusters.erase(it);   }
            else
            {   it++;   }
        }
        
        std::cout << "Cluster Check : " << clusters.size() << "\n";
        std::cout << "IR Brightness : " << irBrightness << "\n";
        std::cout << "UV Check : " << uvBrightness << "\n";
        //showImage(t, "blah", 1);

        if ( (clusters.size() < MaxClusters) && (uvBrightness > 20) && (irBrightness > 240) )
        {
            // Document is fake
            return true;
        }
    }	
    // Document is not fake
    return false;
}