#include <android/log.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

int main()
{
    // get Mat from raw address

    Mat srcMat = cv::imread("/data/a.jpg", IMREAD_COLOR);

    imwrite("/data/b.jpg", srcMat.clone());

}
