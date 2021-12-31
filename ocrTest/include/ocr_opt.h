#ifndef ZYB_OCR_OPT_H
#define ZYB_OCR_OPT_H
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/highgui.hpp> 
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <pthread.h>
#include <sys/time.h>
#include <chrono> 
#include <map>

//版本号
#define MAJOR_VERSION 0
#define MINOR_VERSION 1 
//#define MINOR_VERSION 0 
//#define SUBMINOR_VERSION 7
#define SUBMINOR_VERSION 3 


//是否进行框扩充
//#define TEST_MODE
#define BBOX 1
//#define SILIENT

//#define SHORT_RECOG
#define DIRECT_RECOG
//#undef NO_SILENT

using namespace cv;
using namespace std;

//中文 英文
enum SUBJECT_TYPE{ENGLISH, CHINESE};
//检测算法
enum DETECT_NET{BASIC, CT, DB};
//推理框架
enum FRAME{NCNN, DEFAULT_FRAME};
//识别算法
enum RECOG_NET{CRNN};
//解析配置文件的方式为根据关键词进行字符串拆分
class Opt{
public:
	Opt(){}
	Opt(const char* init_file_path){
		if (!init(init_file_path)){
			printf("init all ocr-sys failed %s \n", init_file_path);
			exit(-1);
		}
	}
	const vector<string> default_detect_method{"Basic", "CenterNet", "Dbnet"};
	const vector<DETECT_NET> default_detect_method_val{BASIC, CT, DB};
	const vector<string> default_frame{"default", "ncnn"};
	const vector<FRAME> default_frame_val{DEFAULT_FRAME, NCNN};
	const vector<string> default_recog_method{"crnn"};
	const vector<RECOG_NET> default_recog_method_val{CRNN};
	const vector<string> default_is_mix{"true", "false"};
	const vector<bool> default_is_mix_val{true, false};
	const string version_key = "version";

	const string detect_method_key = "detect_method";
	const string detect_frame_key = "detect_frame";
	const string detect_model_path_key = "detect_model_path";

	const string is_mix_recog_key = "is_mix_recog";
	const string recog_method_key = "recog_method";
	const string recog_frame_key = "recog_frame";
	const string recog_model_path_key = "recog_model_path";

	const string en_recog_model_method_key = "en_recog_model_method";
	const string en_recog_frame_key = "en_recog_frame";
	const string en_recog_model_path_key = "en_recog_model_path";

	const string g_debug_mode_key = "g_debug_mode";
	const string output_dir_key = "test_output_dir";

	const string collect_key = "collect";

	const string key_seg = ":";
	const string detail_seg = ",";


	bool init(const char* init_file_path);
	string version = "1.0";	
	DETECT_NET detect_method = BASIC;
	FRAME          detect_frame  =  NCNN;
	vector<string> detect_model_path;
	
	bool is_mix_recog = false;
	RECOG_NET recog_method = CRNN;
	FRAME recog_frame = NCNN;
	vector<string> recog_model_path;

	RECOG_NET en_recog_method = CRNN;
	FRAME en_recog_frame = NCNN;
	vector<string> en_recog_model_path;	

	bool isFastRecog = false;
	RECOG_NET fast_recog_method = CRNN;
	FRAME fast_recog_frame = NCNN;
	vector<string> fast_recog_model_path;

	bool g_debug_mode = false;

	string output_dir = "./txts/";

	string collect = "both";

	void print();
};

class sRect{
public:
	sRect(std::vector<cv::Point> &pts, cv::Vec4i h, int i){ 
		if (pts.size() < 4){ 
			isUsed = true;
		}else{
			points.assign(pts.begin(), pts.end());
			rect = cv::boundingRect(points);
			index = i;
			hie = h;
			isUsed = false;
		}   
    }   
	sRect(Rect l){
		rect = l;
	}
	sRect& operator=(sRect const& others){
		img 	 = others.img.clone();
		rect 	 = others.rect;
		points 	 = others.points;
		index 	 = others.index;
		isUsed 	 = others.isUsed;
		hie 	 = others.hie;
		prob 	 = others.prob;
		category = others.category;
		boxPoints= others.boxPoints;
		contours = others.contours;
		centerYDist = others.centerYDist;
		return *this;
	}
	Mat img;
	Rect rect;
	vector<Point> points; //轮廓点
	vector<Point> boxPoints;//四个顶点
	vector<Point> contours;//连通域点
	int index; //索引
	bool isUsed = false;
	Vec4i hie; //连通域算法的索引
	float prob;
	int category; //分类
	float centerYDist = 1000.0;
};
//存储单字识别结果 概率 文本
class Word{
public:
	float prob;
	string word;
	Word(){}
	Word(string w, float p):prob(p), word(w){}
};

typedef struct _OcrWord{
	_OcrWord(string &s, float p):str(s), prob(p){}
	string str;
	float prob;
}OcrWord;

class Object{
};

class Factory{ 
	public: 
	   Object* create(const char* modelPath, bool sign){} 
}; 

#endif
