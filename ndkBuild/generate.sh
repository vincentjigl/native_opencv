#/bin/bash

export ANDROID_SDK=/home/vincent/Android/Sdk
export ANDROID_NDK=/home/vincent/env/ndk/android-ndk-r17c
export PATH=$PATH:$ANDROID_NDK

rm -r build
mkdir build 

ndk-build -j32 NDK_PROJECT_PATH=build/ndk NDK_LIBS_OUT=build/jniLibs APP_BUILD_SCRIPT=Android.mk NDK_APPLICATION_MK=Application.mk

