#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    // get Mat from raw address
    int colormap[] = {
	    COLORMAP_AUTUMN, 
	    COLORMAP_BONE,
	    COLORMAP_JET, 	
	    COLORMAP_WINTER , 
	    COLORMAP_RAINBOW ,
	    COLORMAP_OCEAN ,
	    COLORMAP_SUMMER , 
	    COLORMAP_SPRING , 
	    COLORMAP_COOL , 
	    COLORMAP_HSV , 
	    COLORMAP_PINK , 
	    COLORMAP_HOT , 
	    COLORMAP_PARULA, 
	    COLORMAP_MAGMA , 
	    COLORMAP_INFERNO , 
	    COLORMAP_PLASMA , 
	    COLORMAP_VIRIDIS , 
	    COLORMAP_CIVIDIS , 
	    COLORMAP_TWILIGHT , 
	    COLORMAP_TWILIGHT_SHIFTED , 
	    COLORMAP_TURBO , 
	    COLORMAP_DEEPGREEN 
    };

    Mat dstMat;
    Mat srcMat = cv::imread(argv[1], IMREAD_COLOR);

    int index=0;
    while(true){
	int c= waitKey(2000);
	if(c== 27) //quit
		break;
    	applyColorMap(srcMat, dstMat, colormap[index++%22]);
	imshow("color style", dstMat);
    }
    //imwrite("/data/b.jpg", srcMat.clone());
    return 0;
}
