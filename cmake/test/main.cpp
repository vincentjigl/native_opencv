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
        return 0;
    }

    /*
#ifdef HAVE_TBB
	outLinePosStr(1, "HAVE_TBB");
#else
	outLinePosStr(0, "NO_TBB");
#endif
*/

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
			char filename[64];
			time_t t = time(NULL); //获取目前秒时间
            tm* local = localtime(&t); //转为本地时间
            strftime(filename, 64, "/tmp/saved_imgs/st%m%d%H%M%S.yuv", local);
            vector<int> params;// = std::vector<int>();
            //imwrite(filename, matdst, params);
 			imwrite("stitched.jpg", matdst);
	}

	if(bSaveImages){	// 保存扫描的图片
		MyMat::saveImages(vMatsrc, "saved_imgs");
	}

	ivs->clear();
   
	delete ivs;
    //outInt3(HAND_MODE, bSaveImages, 34);
    cout << endl;
    return 0;
}
