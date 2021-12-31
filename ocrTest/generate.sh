#/bin/bash

export ANDROID_NDK=/home/vincent/env/ndk/android-ndk-r17c

rm -r build
mkdir build && cd build 

cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI="armeabi-v7a" \
    -DANDROID_NDK=$ANDROID_NDK \
    -DANDROID_PLATFORM=android-21 \
    -DANDROID_STL=c++_shared \
    -DOpenCV_DIR=/home/vincent/env/opencv/opencv-4.4.0-android-sdk/sdk/native \
    ..
    #-DANDROID_ABI="arm64-v8a" \
    #-DOpenCV_DIR=./opencvsdk440/native \
    #-DHAVE_OPENMP=1 \

make -j20

