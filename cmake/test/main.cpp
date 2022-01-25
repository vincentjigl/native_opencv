#include "../lib/VideoStitch.h"
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

IVideoStitch *ivs, *givs;
using namespace cv;
// 缓存的图像序列
std::vector<cv::Mat> vMatsrc;

int test_random_stitch();

static int imgproc(int height, int width, char* buff, void *arg)
{
	cv::Mat matsrc(height, width, CV_8UC1, buff);
	vMatsrc.push_back(matsrc.clone());

	return 0; 
}

static void* captureThread2(void *arg)
{
	//ivs->v4l2_capture_start(imgproc);
	   {
	        for(int i=0; i < 120; i++)
	        {
                int w=640, h=480;
                unsigned char* inputY =(unsigned char*)malloc(w*h);
                FILE* fp=fopen("/data/stitch-in.yuv", "r");
                fread(inputY, w*h, 1, fp);
                //fseek(fp, w*h/2, SEEK_CUR);
	            printf("stitch-in frame %d\n", i);

		        cv::Mat frame(w, h, CV_8UC1, inputY);
	            if(!frame.data)
	            {
	                printf("[mbh] video end! line=34\n");
	                break;
	            }

	            vMatsrc.push_back(frame.clone());
	        }

        	return 0;
        }
}

static void* captureThread3(void *arg)
{
	//ivs->v4l2_capture_start(imgproc);
	        {
	        int i=0;
	        char filename[88];
	        cv::Mat imageYUV;

	        for(; i < 120; i++)
	        {
	    		sprintf(filename, "/data/saved_jpg/st%d.jpg", i);
	            printf("%s\n", filename);
		        cv::Mat frame = imread(filename, IMREAD_GRAYSCALE);
	            if(!frame.data)
	            {
	                printf("[mbh] video end! line=34\n");
	                break;
	            }

                //cv::cvtColor(frame, imageYUV, CV_BGR2YUV_YV12);
                //imgproc(frame.rows, frame.cols, (char*)imageYUV.data, arg);
                cv::Mat matsrc(frame.rows, frame.cols, CV_8UC1, frame.data);
                vMatsrc.push_back(matsrc.clone());
            }

	        printf("[mbh]v4l2cap_imgs thread end. total frame num = %d\n", i);
        	return 0;
        }
}

static void* captureThread(void *arg)
{
	//ivs->v4l2_capture_start(imgproc);
	        {
	        int i=0;
	        char filename[88];
	        cv::Mat imageYUV;

	        for(; i < 888; i++)
	        {
	    		sprintf(filename, "/data/saved_imgs6/img%05d.jpg", i);
	            printf("%s\n", filename);
		        cv::Mat frame = imread(filename, IMREAD_COLOR);
	            if(!frame.data)
	            {
	                printf("[mbh] video end! line=34\n");
	                break;
	            }

	    		cv::cvtColor(frame, imageYUV, CV_BGR2YUV_YV12);
	            imgproc(frame.rows, frame.cols, (char*)imageYUV.data, arg);
	        }

	        printf("[mbh]v4l2cap_imgs thread end. total frame num = %d\n", i);
        	return 0;
        }
}

// 读取图片测试
int main(int argc, char **argv)
{
	int  HAND_MODE = 1;			// mode=0,左手模式 mode=1,右手模式
	bool bSaveImages = false;	// 保存原始图片
	cv::Mat matdst;

	ivs = new VideoStitch();

	if(argc > 2){
		HAND_MODE = atoi(argv[1]);
		bSaveImages = atoi(argv[2]);
		ivs->imgpath = argv[3];
	}else{
        test_random_stitch();
        return 0;
    }


	outLinePosStr(1, "HAVE_TBB");


    //cv::setNumThreads(4);

	// 1. 打开摄像头
    pthread_t thrd;
    int ret1 = pthread_create(&thrd, NULL, captureThread, NULL);

	printf("\n[mbh]reading images...\n");
	pthread_join(thrd, NULL);

	//ivs->v4l2_capture_stop();

	int nFrame = vMatsrc.size();
	// ivs->clear();
	printf("[mbh]stitching ... nFrame=%d\n", nFrame);
	// printf("uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu\n");
	// Sleep(2000);
	// return 0;

	//struct timeval tv11, tv12;
    //gettimeofday(&tv11,NULL);

	// 2. 拼接
    cv::Rect roi(200,60,240,350); 
	for(int i=0; i < nFrame; i++){
        cv::Mat tmp = vMatsrc[i](roi);
		int mVal = ivs->stitch(tmp, HAND_MODE);
		// Sleep(6);
	}

	//gettimeofday(&tv12,NULL);
	//double stitchTime = (tv12.tv_sec*1000*1000 + tv12.tv_usec) - (tv11.tv_sec*1000*1000 + tv11.tv_usec);
	//printf("[mbh]stitch thread end. stitchTime = %.1fms frame num= %d, %.1fus/frame\n", stitchTime/1000, nFrame, stitchTime/nFrame);

	// 3. 获取拼接图片
    //gettimeofday(&tv11,NULL);
	ivs->getStitchedImage2(matdst, 2);
	//gettimeofday(&tv12,NULL);
	//stitchTime = (tv12.tv_sec*1000*1000 + tv12.tv_usec) - (tv11.tv_sec*1000*1000 + tv11.tv_usec);
	//outDouble1(stitchTime/1000);

    // gettimeofday(&tv11,NULL);
	// ivs->getStitchedImage(matdst, 0);
	// gettimeofday(&tv12,NULL);
	// stitchTime = (tv12.tv_sec*1000*1000 + tv12.tv_usec) - (tv11.tv_sec*1000*1000 + tv11.tv_usec);
	// outDouble1(stitchTime/1000);

	if(matdst.empty())
		printf("[mbh]Stitched image is Empty !\n");
	else{
		printf("[mbh]Stitched image: w h = %d %d\n", matdst.cols, matdst.rows);
		#if 0 //def  __x86_64__
			imshow("x86_mat", matdst);
			printf("[mbh]Press any key to quit. %s\n", argv[3]);
			waitKey(0);
		#else
			char filename[64];
			time_t t = time(NULL); //获取目前秒时间
            tm* local = localtime(&t); //转为本地时间
            strftime(filename, 64, "/tmp/saved_imgs/st%m%d%H%M%S.yuv", local);
            vector<int> params;// = std::vector<int>();
            //imwrite(filename, matdst, params);
 			imwrite("stitched.jpg", matdst);
		#endif
	}

	if(bSaveImages){	// 保存扫描的图片
		MyMat::saveImages(vMatsrc, "saved_imgs");
	}

	ivs->clear();
   
	delete ivs;
    outInt3(HAND_MODE, bSaveImages, 34);
    cout << endl;
    return 0;
}

//////////////// 多线程单元测试 /////////////////////////////////

int gStitchCount = 10; // 2000
int gGetimgCount = 1000;
bool bStitchRunning = true;

static void* callStitch(void *arg)
{
	int w =  96;// + rand()%400;
	int h = 128;// + rand()%400;
	int i = 0;
	cv::Mat matsrc = cv::Mat(h, w, CV_8UC1);//, cv::Scalar(rand()%256,rand()%256,rand()%256));

	// 1. 拼接
	for(; i < gStitchCount && bStitchRunning; i++)
	{
		if(i % 10 == 0)
			printf("\ncallStitch %d\n", i);

		matsrc.setTo(rand()%256);
    	circle(matsrc, cvPoint(rand()%w, rand()%h), 20, Scalar(255, 255, 255), -1);
    	circle(matsrc, cvPoint(rand()%w, rand()%h), 20, Scalar(255, 255, 255), -1);
		// imshow("matsrc128", matsrc); waitKey(6);

		givs->stitch(matsrc, 1);
		
		Sleep(6);
	}
	printf("callStitch %d\n", i);

	bStitchRunning = false;
}

static void* callGetImage(void *arg)
{
	int *thid = (int*)arg;
	static int nLastGet = 18;
    cv::Mat matdst;

	for(int i=0; i < gGetimgCount && nLastGet > 0; i++)
	{
		printf("\tcallGetImage %d %d", i, *thid);
		if(!bStitchRunning) printf(" nLastGet=%d", nLastGet--);

		// givs->getStitchedImage2(matdst, 1);
		givs->getStitchedImage2(matdst, rand()%3);

		printf(" w h = %d %d\n", matdst.cols, matdst.rows);
		if(matdst.cols > 30000 || matdst.rows > 30000)	// < 0x7fff
		{
			printf("Warning! stitched image size is too large! Exiting...\n");
			bStitchRunning = false;
		}

		// show image
		if(!matdst.empty() && *thid==0 && i%10==0)	// 不能多线程显示
		{
            int winW = 1500;
            if(matdst.cols > winW)
            {
                int h = matdst.rows*(double)winW/matdst.cols;
                cv::resize(matdst, matdst, cvSize(winW, h));
            }
			//imshow("x86_mat", matdst);
			//waitKey(80);
		}
		
		Sleep(6);
	}
}

int test_random_stitch()
{
	int thid[3];
    pthread_t thrd3, thrdc[3];
	srand(time(0));
	// cv::namedWindow("x86_mat", WINDOW_NORMAL);

	givs = new VideoStitch();

	for(int i=0; i < 3; i++)
	{
		thid[i] = i;
		printf("pthread_create thrdc[%d]\n", i);
        pthread_create(&thrdc[i], NULL, callGetImage, &thid[i]);
		Sleep(10);
	}

    pthread_create(&thrd3, NULL, callStitch, NULL);

	// 退出线程
	printf("waitting exit thrd3.\n");
    pthread_join(thrd3, NULL);
    for(int i=0; i < 3; i++)
    {
        printf("waitting exit thrdc[%d].\n", i);
        pthread_join(thrdc[i], NULL);
    }
	outSuccess(3);
	// waitKey(0);
    // getchar(); 
    printf("thrd3 = %ld\n", thrd3);
        // outInt2((long long)thrd3, 0);
	return 0;
}

//////////////// 多行拼接 /////////////////////////////////

#define	V_PUSH_SET(ii,path)	i=ii;	\
			matImg = imread(path, CV_LOAD_IMAGE_COLOR);		\
			vMatLine.push_back(matImg.clone());					\
			ivs->lineMerge_set(vMatLine, i)

#if 0
int main3(int argc, char **argv)
{
	int 	i=0;
	bool 	bNewLine=true;
	cv::Mat matImg, matdst;
	vector<cv::Mat> vMatLine;
	std::vector<cv::Rect> vrc{cvRect(47, 22, 568, 61), cvRect(202, 45, 372, 68)};

	ivs = new VideoStitch();

	V_PUSH_SET(3, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1119.png");
	V_PUSH_SET(3, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1120.png");
	// V_PUSH_SET(3, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1121.png");
	// V_PUSH_SET(3, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1122.png");
	// V_PUSH_SET(3, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1123.png");
	// V_PUSH_SET(3, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1124.png");

	// i=3;
	// for(int j=0; j < 3; j++)
	// {
	// 	cv::Mat mattmp;
	// 	vMatLine.push_back(mattmp.clone());
	// }
	// ivs->lineMerge_set(vMatLine, i);

	V_PUSH_SET(4, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1599.png");
	V_PUSH_SET(4, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1600.png");
	V_PUSH_SET(4, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1601.png");
	V_PUSH_SET(4, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1602.png");
	V_PUSH_SET(4, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1603.png");
	V_PUSH_SET(4, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1604.png");
	V_PUSH_SET(5, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img1848.png");
	V_PUSH_SET(6, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img2124.png");
	V_PUSH_SET(6, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img2125.png");
	V_PUSH_SET(6, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img2126.png");
	V_PUSH_SET(6, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img2127.png");
	V_PUSH_SET(6, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img2128.png");
	V_PUSH_SET(6, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img2129.png");
	V_PUSH_SET(6, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img2130.png");
	V_PUSH_SET(8, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img687.png");
	V_PUSH_SET(8, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img688.png");
	V_PUSH_SET(8, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img689.png");
	V_PUSH_SET(8, "/home/agl/lxr/imgs/拼接采集的样本/pic/detect_img690.png");

	int ret2 = ivs->lineMerge_get(matdst);

	// outInt2(ret1, ret2);
	outMatSize(matdst);
	if(!matdst.empty())
	{
		// matImg = imread("/mnt/mbhshare/ocr_pic/detect_img1123.png", CV_LOAD_IMAGE_GRAYSCALE);
		// if(!matImg.empty())
		// 	imshow("181matImg", matImg);
		outMatSize(matImg);

		imshow("125matdst", matdst);
		waitKey(0);
	}

	delete ivs;
	return 0;
}
#endif
