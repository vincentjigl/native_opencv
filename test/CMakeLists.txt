cmake_minimum_required(VERSION 3.4.1)

include_directories(${OpenCV_DIR}/jni/include)
add_library( lib_opencv SHARED IMPORTED )
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${OpenCV_DIR}/libs/${ANDROID_ABI}/libopencv_java4.so)

#set(opencvstatic ${OpenCV_DIR}/staticlibs/${ANDROID_ABI}/libopencv_imgcodecs.a)

#add_library(native-lib SHARED native-lib.cpp)
add_executable(hello test.cpp)


find_library(log-lib log)


#target_link_libraries(native-lib lib_opencv ${log-lib})
target_link_libraries(hello lib_opencv ${log-lib} 
    ${OpenCV_DIR}/staticlibs/${ANDROID_ABI}/libopencv_highgui.a 
    ${OpenCV_DIR}/staticlibs/${ANDROID_ABI}/libopencv_imgcodecs.a)
