#/bin/bash

export ANDROID_NDK=/home/vincent/env/ndk/android-ndk-r21e

rm -r build
mkdir build && cd build 

cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI="armeabi-v7a" \
    -DANDROID_NDK=$ANDROID_NDK \
    -DANDROID_PLATFORM=android-21 \
    -DOpenCV_DIR=/home/vincent/enc/opencv/opencv-3.4.16-android-sdk/sdk/native \
    ..

make -j60

