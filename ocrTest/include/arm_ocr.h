/*
此文件提供扫描笔ocr系统的入口
ocr基本功能： 输入一张图片，得到检测结果，识别结果
调用方式： 初始化单例（指定模型路径方式，指定配置文件方式，上线模式（什么都不指定））-> 调用接口获取结果
 */

#ifndef ARM_OCR_H
#define ARM_OCR_H
#include <future>
#include "ocr_opt.h"
#include "ocr_recog.h"
#include "ocr_detect.h"

void filterBadFEWords(vector<Word> &res, float prob=0.1);
//网络参数
class NetParam{
public:
	NetParam(){}
	Mat img;
	string str; 
	float prob;
	float thres;
};
class ArmOcrSys{
private:
	//配置文件提取的信息
	Opt opt;
	//识别对象 如果长短模式 则为小模型对象，否则为通用识别对象
	RecogObject* recogInstance;
	//长短模式识别时 长模型对象实例 正常模式不初始化
	RecogObject* longRecogInstance;
	//中英混合识别时 为英文识别实例 正常模式不初始化
	RecogObject* enRecogInstance = nullptr;
	//长短混合识别 异步future
	future<int> _thread;
	//检测对象实例
	DetectObject* detectObject = nullptr;
	//回调函数 未使用
	function<void(Mat&, string&, float&)> recogCallBack = [](Mat&, string&, float&){};
	//检测图像 
	//img: 待检测图像 
	//vector<Mat> &imgOut 检测得到的分片结果
	void detectImg(Mat &img, vector<Mat> &imgOut);
	//文本分类 默认分类为中文
	SUBJECT_TYPE subject = CHINESE;
	//是否是直接识别 未使用
	bool isDirectRecog = false;
	//未使用
	float lprob = 0.0;
	//上线初始化函数
	void init();
public:
	ArmOcrSys();
	//单例模式删除接口
	ArmOcrSys& operator=(ArmOcrSys const& others)=delete;
	//单例模式删除接口
	ArmOcrSys(ArmOcrSys const& others) = delete;
	//debug 模式 下的指定模型路径进行初始化
	ArmOcrSys(string shortModelPath, string longModelPath){
		if (!init_ocr(shortModelPath, longModelPath)){
			//logTrace("ArmOcrSys init failed \n");
			exit(-1);
		}
	}
	//通过配置文件进行初始化
	ArmOcrSys(string configPath){
		if (!init_ocr(configPath)){
			//logTrace("init by configPath %s failed \n", configPath.c_str());
			exit(-1);
		}
	}
	//通过指定长短模型路径的方式进行单例初始化
	static ArmOcrSys& get_instance(string shortModelPath, string longModelPath){
		static ArmOcrSys instance(shortModelPath, longModelPath);
		return instance;
	}
	//通过指定配置文件路径的方式进行单例初始化
	static ArmOcrSys& get_instance(string configPath){
		static ArmOcrSys instance(configPath);
		return instance;
	}
	//线上初始化 模型从文件直接读取
	static ArmOcrSys& get_instance(){
		static ArmOcrSys instance;
		return instance;
	}
	//指定长短模型路径初始化
	bool init_ocr(string shortModelPath, string longModelPath);
	//未使用
	bool init_ocr_naive(string configPath);
	//指定配置文件路径的初始化方式
	bool init_ocr(string configPath);
	//整图识别接口 实际线上被调用接口
	//img: 输入待ocr的图片
	//imgOut： 检测结果
	//str：识别结果输出
	//prob： 识别概率输出
	//isLast： 是否是最后一次识别 未启用
	void getBigImgOcrRes(Mat &img, Mat &imgOut, string &str, float &prob, bool isLast = false);	
	//检测图片 
	//input：待检测图片
	//output：输出检测结果，为检测区域裁剪之后的结果
	void detectImg(Mat &input, Mat &output);
	//长短模型混合使用时使用，上线 未启用 清除缓存 退出后台小图识别线程 析构时使用 
	//注册回调函数 未启用
	void setOcrCallBack(function<void(Mat&, string& str, float& prob)> func){
		recogCallBack = func;
	}
	//debug版本使用 与 上线函数同功能 上线可删除  
	void getBigImgOcrRes(Mat &img, Mat &imgOut, string &str, float &prob, string sid);
	//析构 退出后台线程 网络请求资源释放
	~ArmOcrSys(){
	}
};
#endif
