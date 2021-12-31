#ifndef RECOG_ARM_OCR_H
#define RECOG_ARM_OCR_H
#include "ocr_opt.h"
#include <fstream>
//识别对象
class RecogObject:public Object{
public:
	RecogObject(){} 
	RecogObject& operator=(RecogObject const& others)=delete;
	RecogObject(RecogObject const& others) = delete;
	RecogObject(const char* modelPath){}
	virtual bool loadModel(const char* modelPath) = 0;
	virtual void recogImg(Mat img, vector<Word> &res)= 0;
};
class RecogFactory:public Factory{
public:
	//ncnn 通过指定ncnn 的模型路径创造单例指针
	 RecogObject* create(const char* paramPath, const char* binPath);
	 //上线版本 直接从文件创造识别单例指针
	 RecogObject* _create();
};

#endif
