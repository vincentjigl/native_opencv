/*
 * test.cpp
 * Copyright (C) 2021 vincent <vincent@vincent-Super-Server>
 *
 * Distributed under terms of the MIT license.
 */
#include<stdio.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
using namespace cv;

int main(){
    Mat matdst;
    printf("start read jgp\n");
    char filename[64];
    sprintf(filename, "/data/a.jpg");
    printf("%s\n", filename);
    cv::Mat frame = imread(filename, IMREAD_COLOR);
    //imshow("x86_mat", frame);
    imwrite("/data/b.jpg", frame.clone());
    printf("finish read jgp\n");
}
