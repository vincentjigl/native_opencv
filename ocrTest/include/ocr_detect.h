#ifndef ZYB_OCR_DETECT_H
#define ZYB_OCR_DETECT_H
#include "ocr_opt.h"

class DetectObject:public Object{
public:
	//检测算法 传统的图像检测算法 centernet检测算法， dbnet检测算法
	enum DETECT_METHOD{BASIC, CENTERNET, DBNET};
	DETECT_METHOD method_type;
	//虚函数
	//img：传入图像
	//imgOut：检测结果
	//res：检测框/mask
	virtual void detect(Mat img, Mat& imgOut, vector<sRect> &res){}
};
class DetectFactory:public Factory{
public:
	//产生传统图像检测算法的指针
	DetectObject* create();	
	//获取基于ncnn推理的检测单例指针
	DetectObject* create(const char* param, const char* bin,  DETECT_NET net = CT);
	//线上： 获取基于ncnn的centernet检测推理指针
	DetectObject* _create();
};

//传统图像方法进行空格检测
void detectBlankRect(Mat &img, vector<sRect> &detectRes, Mat imgBw=Mat());
#endif
