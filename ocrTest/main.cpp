#include "arm_ocr.h"

using namespace std;
using namespace cv;

int main(int argc, char * argv[]){
	if(argc != 2){
		cout<<"Usage: bash *.exec inputImg"<<endl;
		exit(1);
	}
	Mat img = imread(argv[1], 0);
	cout<<img.rows<<" x "<<img.cols<<endl;

	string str;
	float prob;
	Mat imgOut;

	ArmOcrSys &ocrSys = ArmOcrSys::get_instance();
	ocrSys.getBigImgOcrRes(img, imgOut, str, prob, true);
	
	cout<<"result: "<<str.c_str()<<" prob: "<<prob<<endl;

}
