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
struct Dist {
    bool operator()(const Point& a, const Point &b) {
           return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y)) < 15;
    }
};
struct ColorDist {
    bool operator()(const Vec3b& a, const Vec3b &b) {
        return norm(a,b) < 65; // experimental value.
    }
};

Mat m=imread("ARH_CUT-Infra-n5IS2-2020-02-10-13-39-02.jpg",1);
resize(m,m,Size(), 0.2, 0.2); // i'm in a hurry ;)
cerr << m.total() << " pixels." << endl;

// pack our pixels into a vector (better idea, anyone ?)
vector<Vec3f> data;
data.insert(data.begin(), m.begin<Vec3b>(), m.end<Vec3b>());

// partition labels:
vector<int> labels;
int k = partition(data, labels, ColorDist());
cerr << k << " clusters." << endl;


}