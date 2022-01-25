/*
 * test.cpp
 * Copyright (C) 2022 vincent <vincent@vincent-Super-Server>
 *
 * Distributed under terms of the MIT license.
 */

#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
void testyuv();
void yuv2jpg(){
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
        char filename[64];
        sprintf(filename, "/data/saved_jpg/st%d.jpg", i);
        imwrite(filename, frame);

    }
}


int main(){
    //testyuv();
    yuv2jpg();

    return 0;
}

void testyuv(){
    int w=640, h=480;
    unsigned char* inputY=(unsigned char*)malloc(w*h);
    unsigned char* inputY2=(unsigned char*)malloc(w*h*3/2);

    FILE* fp=fopen("/data/test.yuv", "r");
    fread(inputY, w*h, 1, fp);

    Mat m1(h, w, CV_8UC1, inputY);
    imwrite("/data/m1.jpg", m1);
    FILE* fm1=fopen("/data/m1.yuv", "ab+");
    fwrite(m1.data, w*h, 1, fm1);
    fclose(fm1);

    fseek(fp, w*h, SEEK_SET);
    unsigned char* inputUV=(unsigned char*)malloc(w*h/2);
    fread(inputUV, w*h/2, 1, fp);
    FILE* fm11=fopen("/data/m11.yuv", "ab+");
    fwrite(m1.data, w*h, 1, fm11);
    fwrite(inputUV, w*h/2, 1, fm11);
    fclose(fm11);
    free(inputUV);



    fseek(fp, 0L, SEEK_SET);
    Mat m2(h, w, CV_8UC3, inputY2);
    imwrite("/data/m2.jpg", m1);
    FILE* fm2=fopen("/data/m2.yuv", "ab+");
    fwrite(m2.data, w*h*3/2, 1, fm2);
    fclose(fm2);

    fclose(fp);
    free(inputY);
    free(inputY2);
    inputY=NULL;
    inputY2=NULL;
}



