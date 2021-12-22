LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#OPENCVROOT:= /home/vincent/env/opencv/opencv-4.4.0-android-sdk
OPENCVROOT:= ../opencvsdk440
OPENCV_CAMERA_MODULES:=off
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
#include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk
include ${OPENCVROOT}/native/jni/OpenCV.mk

LOCAL_SRC_FILES := test/main.cpp lib/v4l2cap_img.cpp
#LOCAL_SRC_FILES := test/test.cpp #lib/v4l2cap_img.cpp

LOCAL_LDLIBS += -llog -landroid 
LOCAL_MODULE := test 

#LOCAL_LDLIBS += -L$(OPENCVROOT)/sdk/native/staticlibs/${APP_ABI}
LOCAL_LDLIBS += -L$(OPENCVROOT)/native/staticlibs/${APP_ABI}
LOCAL_LDLIBS += -lopencv_imgcodecs -lopencv_core  -lopencv_highgui
include $(BUILD_EXECUTABLE)
