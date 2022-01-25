#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

class QuickDemo{
public:
	void colorMap_demo(Mat &image);
	void bitwise_demo(Mat &image);
	void channels_demo(Mat &image);
	void inrange_demo(Mat &image);
	void pixel_statistic_demo(Mat &image);


};

void QuickDemo::pixel_statistic_demo(Mat &image){
	double maxv, minv;
	Point minLoc, maxLoc;
	vector<Mat> mv;
	split(image, mv);

	for(int i=0; i< mv.size(); i++){
		minMaxLoc(mv[i], &minv, &maxv, &minLoc, &maxLoc, Mat());
		std::cout <<"No. channels:"<< i << " min value:" << minv << " max value:" << maxv << std::endl;
		//minMaxLoc(mv[i], &minv, &maxv, &minLoc, &maxLoc, Mat());
		//cout<<"No.channel:" <<i << " min value:"<<minv<<" max value:" << maxv<<endl;
	}

	Mat mean, stddev;
	meanStdDev(image, mean, stddev); 
	cout<<"means: " <<mean << "stddev: " <<stddev<<endl;

}

void QuickDemo::inrange_demo(Mat &image){
	imshow("background", image);
	Mat hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);
	Mat mask;
	inRange(hsv, Scalar(35, 43, 46), Scalar(77, 255,255), mask);

	Mat redback = Mat::zeros(image.size(), image.type());
	redback = Scalar(40, 40, 200);
	bitwise_not(mask, mask);
	imshow("mask", mask);

	image.copyTo(redback, mask);
	imshow("roi", redback);

	waitKey(20000);

}

void QuickDemo::channels_demo(Mat &image){
	vector<Mat> mv;
	split(image, mv);
	imshow("green", mv[1]);
	imshow("red", mv[2]);

	Mat dst;
	mv[1]=0;
	mv[2]=0;
	merge(mv, dst);
	imshow("blue", dst);
	
	waitKey(20000);

}


void QuickDemo::bitwise_demo(Mat &image){
	Mat m1 = Mat::zeros(Size(256,256), CV_8UC3);
	Mat m2 = Mat::zeros(Size(256,256), CV_8UC3);

	rectangle(m1, Rect(100, 100, 80, 80), Scalar(255,255, 0), -1, LINE_8, 0);
	rectangle(m2, Rect(150, 150, 80, 80), Scalar(0,255, 255), -1, LINE_8, 0);

	imshow("m1", m1);
	imshow("m2", m2);

	Mat dst;
	bitwise_xor(m1, m2, dst);
	imshow("pixel bit and", dst);
	waitKey(20000);
	
	
}

void QuickDemo::colorMap_demo(Mat &image){
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
    int index=0;
    while(true){
	int c= waitKey(2000);
	if(c== 27) //quit
		break;
    	applyColorMap(image, dstMat, colormap[index++%22]);
	imshow("color style", dstMat);
    }
 
}

int main(int argc, char* argv[])
{
    // get Mat from raw address
    QuickDemo * qd= new QuickDemo();
    Mat srcMat = cv::imread(argv[1], IMREAD_COLOR);
    //qd->colorMap_demo(srcMat);
    //qd->bitwise_demo(srcMat); 
    //qd->channels_demo(srcMat);
    //qd->inrange_demo(srcMat);
    qd->pixel_statistic_demo(srcMat);

    delete qd;
    qd = NULL;
   //imwrite("/data/b.jpg", srcMat.clone());
    return 0;
}
