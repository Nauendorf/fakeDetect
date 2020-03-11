/*
    Face is always in the same location between passports.
    UV feature is always different.
    Test whether the brightest region is inside the area where we expect to see the UV feature.
    Count the ratio of bright pixels nearby the brightest to ensure we found the UV feature. 
*/

#include "class.fakeDetect.cpp"


int main(int argc, char*argv[])
{
    Passport *pp = new Passport();
    Configuration *conf = new Configuration();
    
    // Variables
    int fakeConf = 0;
    double ir_min_val;
    double ir_max_val;
    double uv_min_val;
    double uv_max_val;
    double hsv_min_val;
    double hsv_max_val;

    cv::Mat ir_blur;
    cv::Mat uv_blur;
    cv::Mat uvImg_hsv;
    cv::Mat hsv[3];

    cv::Point uv_min_loc(0,0);
    cv::Point uv_max_loc(0,0);
    cv::Point ir_min_loc(0, 0);
    cv::Point ir_max_loc(0,0);
    cv::Point hsv_min_loc(0, 0);
    cv::Point hsv_max_loc(0,0);

    std::string uvImagePath = argv[1];
    std::string irImagePath = argv[2];

    cv::Mat uvImg_mat = cv::imread(uvImagePath);
    cv::Mat irImg_mat = cv::imread(irImagePath);
  
    cv::Rect uvFrame = pp->getFrame(uvImg_mat.size, 0.05);
    cv::Rect irFrame = pp->getFrame(irImg_mat.size, 0.05);
    
    // These become the main Mat's we are processing
    cv::Mat uvImg = uvImg_mat(uvFrame);
    cv::Mat irImg = irImg_mat(irFrame);
    
    float uvResult = pp->getUvPercentage(uvImg);    
    float irResult = pp->getIrBrightness(irImg);
    std::cout << "UV Brightness  " << uvResult << "\n";
    std::cout << "IR Brightness  " << irResult << "\n";

    // Convert the UV layer to HSV and grab the second channel
    cv::cvtColor(uvImg, uvImg_hsv, cv::COLOR_BGR2HSV);
    cv::split(uvImg_hsv, hsv);
    cv::Mat ch2 = hsv[2];

    // Blur all of our Mat's to reduce pixel colour/brightness variance 
    cv::medianBlur(uvImg, uv_blur, 31);
    cv::medianBlur(irImg, ir_blur, 31);
    cv::medianBlur(ch2, ch2, 31);

    // Convert blurs to gray to make it easier to identify the brightest pixel
    cv::cvtColor(ir_blur, ir_blur, cv::COLOR_BGR2GRAY);
    cv::cvtColor(uv_blur, uv_blur, cv::COLOR_BGR2GRAY);
    cv::minMaxLoc(ir_blur, &ir_min_val, &ir_max_val, &ir_min_loc, &ir_max_loc); 
    cv::minMaxLoc(uv_blur, &uv_min_val, &uv_max_val, &uv_min_loc, &uv_max_loc);
    cv::minMaxLoc(ch2, &hsv_min_val, &hsv_max_val, &hsv_min_loc, &hsv_max_loc);

    // Lets draw some circles for debug
    cv::circle(ch2, hsv_max_loc, 300, cv::Scalar(245, 66, 188), 5);
    cv::circle(uvImg, uv_min_loc, 50, cv::Scalar(66, 245, 215), 5);
    cv::circle(uvImg, uv_max_loc, 50, cv::Scalar(245, 66, 188), 5);
    cv::circle(irImg, ir_min_loc, 50, cv::Scalar(66, 245, 215), 5);
    cv::circle(irImg, ir_max_loc, 50, cv::Scalar(245, 66, 188), 5);

    // Rect for checking whether the brightest pixel is inside the known UV feature area
    cv::Rect feature_rect = cv::Rect(1110, 130, 700, 600);
    cv::Mat feature = ch2(feature_rect);
    cv::Rect face_rect = cv::Rect(60, 200, 540, 720);
    cv::Mat face_mat = irImg(face_rect);
    // Check if the brightest pixel is inside the feature area
    bool uv_isInside = pp->isInsideRect(feature_rect, hsv_max_loc, uvImg.size);
    bool ir_isInside = pp->isInsideRect(face_rect, ir_min_loc, face_mat.size);
    //std::cout << uv_min_loc << "\n";
    //std::cout << uv_max_loc << "\n";
    //std::cout << ir_min_loc << "\n";
    //std::cout << ir_max_loc << "\n";

    float irfaceBrightness = pp->getIrBrightness(face_mat);
    //pp->showImage(face_mat);
    std::cout << "ir Face Brightness  " << irfaceBrightness << "\n";

    // Increase fake confidence based on checks targeting different metrics
    uvResult > 50 ? (fakeConf++) : 0;
    irResult > 160 ? (fakeConf++) : 0;
    irfaceBrightness > 170 ? (fakeConf++) : 0;
    uv_isInside == false ? (fakeConf+=2) : 0;
    ir_isInside == false ? (fakeConf+=2) : 0;

    if ( fakeConf < 5 )
    {
        // Feature is where we expect it to be so return false, it's not a fake.
        std::cout << "Fake Confidence  :  " << fakeConf << "\n";
        std::cout << "UV isInside value  :  " << uv_isInside << "\n";
        std::cout << "IR isInside value  :  " << ir_isInside << "\n";
        std::cout << "it's NOT a fake..." << "\n";
        std::cout << uvImagePath << "\n";
        pp->showImage(uvImg, 1);
        pp->showImage(irImg, 1);
        return false;
    }
    // else return true, it's a fake.
    std::cout << "Fake Confidence  :  " << fakeConf << "\n";
    std::cout << "UV isInside value  :  " << uv_isInside << "\n";
    std::cout << "IR isInside value  :  " << ir_isInside << "\n";
    std::cout << "IT'S A FAKE!!!" << "\n";
    std::cout << uvImagePath << "\n";
    pp->showImage(uvImg, 1);
    pp->showImage(irImg, 1);
    return true;
}



