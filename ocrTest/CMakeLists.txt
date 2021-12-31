cmake_minimum_required(VERSION 3.0.0)

#set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ")#-O3
project(linux_arm_ocr VERSION 0.1.0)
#set(CMAKE_C_FLAGS "-fopenmp")
#set(CMAKE_CXX_FLAGS "-std=c++11 -fopenmp -g")

include_directories(${OpenCV_DIR}/jni/include)
add_library( lib_opencv SHARED IMPORTED )
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${OpenCV_DIR}/libs/${ANDROID_ABI}/libopencv_java4.so)

set(OCR_INCLUDE_DIRS "./include/")
set(OCR_LIBS ${PROJECT_SOURCE_DIR}/lib/libocr_util.so)
set(SRC_FILES "./main.cpp")

find_library(log-lib log)

include_directories(${OCR_INCLUDE_DIRS})
add_executable(ocr_test ${SRC_FILES})
target_link_libraries(ocr_test -landroid ${log-lib} ${OCR_LIBS} lib_opencv -fopenmp)
