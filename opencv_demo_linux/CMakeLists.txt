cmake_minimum_required(VERSION 3.4.1)

SET (OPENCV_INCLUDE_PATH /usr/local/include /usr/local/include/opencv4 /usr/local/include/opencv2)
SET (OpenCV_LIBS opencv_core opencv_highgui opencv_imgproc opencv_flann opencv_features2d opencv_imgcodecs opencv_photo opencv_stitching opencv_video opencv_videoio)


message("opencv include path: " ${OPENCV_INCLUDE_PATH})
SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

INCLUDE_DIRECTORIES (${OPENCV_INCLUDE_PATH})
#LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/bin) # no need: ${OPENCV_LIB_PATH}

ADD_EXECUTABLE (test test.cpp)
TARGET_LINK_LIBRARIES (test ${OpenCV_LIBS})
message("Build : " ${CMAKE_BUILD_TYPE})
