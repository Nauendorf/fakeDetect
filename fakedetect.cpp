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
    std::cout << conf->frame_percent << "\n";
    std::cout << conf->blur_ir_ksize << "\n";
    std::cout << conf->blur_uv_ksize << "\n";
    std::cout << conf->debug << "\n";
    // Variables
    double ir_min_val;
    double ir_max_val;
    double uv_min_val;
    double uv_max_val;

    cv::Mat ir_blur;
    cv::Mat uv_blur;

    cv::Point ir_min_loc(0, 0);
    cv::Point ir_max_loc(0,0);

    cv::Point uv_min_loc(0,0);
    cv::Point uv_max_loc(0,0);

    std::string uvImagePath = argv[1];
    std::string irImagePath = argv[2];

    cv::Mat uvImg_mat = cv::imread(uvImagePath);
    cv::Mat irImg_mat = cv::imread(irImagePath);

    
    cv::Rect uvFrame = pp->getFrame(uvImg_mat.size, 0.05);
    cv::Rect irFrame = pp->getFrame(irImg_mat.size, 0.05);
    

    cv::Mat irImg = irImg_mat(uvFrame);
    cv::Mat uvImg = uvImg_mat(irFrame);

    float uvResult = pp->getUvPercentage(uvImg);    
    float irResult = pp->getIrBrightness(irImg);

    //std::cout << uvResult << "\n";
    //std::cout << irResult << "\n";

    cv::medianBlur(irImg, ir_blur, 31);
    cv::medianBlur(uvImg, uv_blur, 31);

    cv::cvtColor(ir_blur, ir_blur, cv::COLOR_BGR2GRAY);
    cv::minMaxLoc(ir_blur, &ir_min_val, &ir_max_val, &ir_min_loc, &ir_max_loc);
    
    cv::cvtColor(uv_blur, uv_blur, cv::COLOR_BGR2GRAY);
    cv::minMaxLoc(uv_blur, &uv_min_val, &uv_max_val, &uv_min_loc, &uv_max_loc);

    cv::circle(uvImg, uv_min_loc, 50, cv::Scalar(66, 245, 215), 5);
    cv::circle(uvImg, uv_max_loc, 50, cv::Scalar(245, 66, 188), 5);

    cv::circle(irImg, ir_min_loc, 50, cv::Scalar(66, 245, 215), 5);
    cv::circle(irImg, ir_max_loc, 50, cv::Scalar(245, 66, 188), 5);

    //std::cout << uv_min_loc << "\n";
    //std::cout << uv_max_loc << "\n";
    //std::cout << ir_min_loc << "\n";
    //std::cout << ir_max_loc << "\n";

    cv::Mat face = irImg(cv::Rect(110, 180, 420, 580));
    float faceBrightness = pp->getIrBrightness(face);
    //std::cout << faceBrightness << "\n";

    cv::imshow("Naive", face);
    cv::waitKey(0);

    cv::resize(uvImg, uvImg, cv::Size(irImg.cols/2,irImg.rows/2));
    cv::imshow("Naive", uvImg);
    cv::waitKey(0);

    cv::resize(irImg, irImg, cv::Size(irImg.cols/2,irImg.rows/2));
    cv::imshow("Naive", irImg);
    cv::waitKey(0);

    if ( (irResult > 160) && (uvResult > 60) )
    {
        if ( (faceBrightness > 170) )
        {
            std::cout << "THIS IS A FAKE!!!!!";
            cv::resize(uvImg, uvImg, cv::Size(irImg.cols/2,irImg.rows/2));
            cv::imshow("Naive", uvImg);
            cv::waitKey(0);

            cv::resize(irImg, irImg, cv::Size(irImg.cols/2,irImg.rows/2));
            cv::imshow("Naive", irImg);
            cv::waitKey(0);
        }
    }
    
}



