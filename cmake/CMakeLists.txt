CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
SET(SRC_PATH ..)
SET (EXECUTABLE_NAME test)

SET(CMAKE_CXX_FLAGS "-Wall -fPIC -Wl,-Bsymbolic -lm -D__STDINT_LIMITS -D__STDINT_MACROS -D__ANDROID__ -DANDROID")
add_library( lib_opencv SHARED IMPORTED )
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${OpenCV_DIR}/libs/${ANDROID_ABI}/libopencv_java3.so)

SET (MY_INCLUDE_PATH ../vcpp15)
#SET (OpenCV_LIBS opencv_core opencv_highgui opencv_imgproc opencv_flann opencv_features2d opencv_imgcodecs opencv_photo opencv_stitching opencv_video opencv_videoio)
SET (OpenCV_LIBS opencv_java3 libopencv_core.a libopencv_highgui.a libopencv_imgproc.a libopencv_flann.a libopencv_features2d.a libopencv_imgcodecs.a libopencv_photo.a libopencv_stitching.a libopencv_video.a libopencv_videoio.a)

SET (SRC_APP ${SRC_PATH}/test/main.cpp ${SRC_PATH}/lib/v4l2cap_img.cpp)

find_library(log-lib log)
find_library(thread-lib pthread)
message("opencv include path: " ${OPENCV_INCLUDE_PATH})
# 添加这句，将lib存放到与执行文件相同的目录下，也可以存放到不同目录。
SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
LINK_DIRECTORIES (${Opencv_DIR}/libs/${ANDROID_ABI} ${Opencv_DIR}/staticlibs/${ANDROID_ABI})

#INCLUDE_DIRECTORIES (${OPENCV_INCLUDE_PATH} ${MY_INCLUDE_PATH} "lib" "/usr/include/x86_64-linux-gnu/curl" "/usr/include/jsoncpp")
INCLUDE_DIRECTORIES (/home/vincent/env/opencv/opencv-3.4.16-android-sdk/sdk/native/jni/include)
INCLUDE_DIRECTORIES (${OPENCV_INCLUDE_PATH} ${MY_INCLUDE_PATH} "lib" ) 
#LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/bin) # no need: ${OPENCV_LIB_PATH}

# 3 生成执行程序
ADD_EXECUTABLE (${EXECUTABLE_NAME} ${SRC_APP})
TARGET_LINK_LIBRARIES (${EXECUTABLE_NAME} ${thread-lib} ${OpenCV_LIBS})
message("Build : " ${CMAKE_BUILD_TYPE})
