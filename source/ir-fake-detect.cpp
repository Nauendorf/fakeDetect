

/*
    Face is always in the same location between passports.
    UV feature is always different.
    Test whether the brightest region is inside the area where we expect to see the UV feature.
    Count the ratio of bright pixels nearby the brightest to ensure we found the UV feature. 
*/

#include "class.ir-fake-detect.cpp"


int main(int argc, char*argv[])
{
    Passport *pp = new Passport();
    
    // Variables
    int fakeConf = 0;
    double ir_min_val;
    double ir_max_val;

    cv::Mat ir_blur;
    cv::Point ir_min_loc(0, 0);
    cv::Point ir_max_loc(0,0);
    std::string irImagePath = argv[1];
    cv::Mat irImg_mat = cv::imread(irImagePath);
    cv::Rect irFrame = pp->getFrame(irImg_mat.size, 0.05);
    
 
    cv::Mat irImg = irImg_mat(irFrame);
       
    float irResult = pp->getIrBrightness(irImg);

 
    cv::medianBlur(irImg, ir_blur, 31);


    cv::cvtColor(ir_blur, ir_blur, cv::COLOR_BGR2GRAY);
    cv::minMaxLoc(ir_blur, &ir_min_val, &ir_max_val, &ir_min_loc, &ir_max_loc); 

    std::vector<cv::Point> neighbours = pp->getNeighbours(ir_min_loc);
    for (int i=0; i<40; i++)
    {
        cv::Point min_loc = neighbours[i];
        cv::circle(irImg, min_loc, 50, cv::Scalar(66, 245, 215), 5);
    }
    std::cout << ir_blur.size[0] << "\n";
    std::cout << ir_blur.size[1] << "\n";


    cv::circle(irImg, ir_min_loc, 50, cv::Scalar(66, 245, 215), 5);
    cv::circle(irImg, ir_max_loc, 50, cv::Scalar(245, 66, 188), 5);

 
    cv::Rect face_rect = cv::Rect(60, 200, 540, 720);
    cv::Mat face_mat = irImg(face_rect);

    bool ir_isInside = pp->isInsideRect(face_rect, ir_min_loc, face_mat.size);
    std::cout << "ir_min_loc  " << ir_min_loc << "\n";
    //std::cout << ir_max_loc << "\n";


    cv::Point pixel_loc = cv::Point(100,100);
    std::cout << "pixel_loc1  " << pixel_loc << "\n";
    cv::Scalar color = irImg.at<cv::Vec3b>(ir_min_loc);
    cv::circle(irImg, ir_min_loc, 50, cv::Scalar(45, 240, 18), 5);


    cv::Vec3b *p = ir_blur.ptr<cv::Vec3b>();
    std::vector<cv::Vec3b> pix(p, p+ir_blur.total());   
    std::vector<int> labels;
    int unique = cv::partition(pix, labels, pp->pixel_equal());




    pp->findDarkPixels(ir_blur, ir_min_val);
    
    std::cout << "test2  " << color.val[0] << "\n";
    std::cout << "test2  " << color.val[1] << "\n";
    std::cout << "test2  " << color.val[2] << "\n";
    std::cout << "test2  " << color.val[3] << "\n";




    float irfaceBrightness = pp->getIrBrightness(face_mat);
    //pp->showImage(face_mat);

    // Increase fake confidence based on checks targeting different metrics
    irResult > 160 ? (fakeConf++) : 0;
    irfaceBrightness > 165 ? (fakeConf++) : 0;
    ir_isInside == false ? (fakeConf+=2) : 0;

    if ( fakeConf < 3 )
    {
        // Feature is where we expect it to be so return false, it's not a fake.
        std::cout << "IR Brightness  " << irResult << "\n";
        std::cout << "ir Face Brightness  " << irfaceBrightness << "\n";
        std::cout << "Fake Confidence  :  " << fakeConf << "\n";
        std::cout << "IR isInside value  :  " << ir_isInside << "\n";
        std::cout << "IR min_val  :  " << ir_min_val << "\n";
        std::cout << "it's NOT a fake..." << "\n";
        pp->showImage(irImg, 1);
        return false;
    }
    // else return true, it's a fake.
    std::cout << "IR Brightness  " << irResult << "\n";
    std::cout << "ir Face Brightness  " << irfaceBrightness << "\n";
    std::cout << "Fake Confidence  :  " << fakeConf << "\n";
    std::cout << "IR isInside value  :  " << ir_isInside << "\n";
    std::cout << "IR min_val  :  " << ir_min_val << "\n";
    std::cout << "it's a FAKE!!..." << "\n";
    pp->showImage(irImg, 1);
    return true;
}



