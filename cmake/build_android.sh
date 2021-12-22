#! /bin/sh
#
# build_android.sh
# Copyright (C) 2021 vincent <vincent@vincent-Super-Server>
#
# Distributed under terms of the MIT license.
#

ABI=armeabi-v7a
NDK=/home/vincent/env/ndk/android-ndk-r17c

cmake -DDEBUG=NO  \
      -DCMAKE_TOOLCHAIN_FILE=${NDK}/build/cmake/android.toolchain.cmake \
      -DANDROID_NDK=$NDK                   \
      -DANDROID_ABI="armeabi-v7a"          \
	  -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9\
	  -DANDROID_NATIVE_API_LEVEL=21\
	  -DANDROID_STL=c++_static\
      -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=./obj/armeabi-v7a \
      -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=./obj/armeabi-v7a \
      ..

#cmake \
#    -DANDROID_ABI="${ABI}" \
#    -DANDROID_PLATFORM=android-21 \
#    -DANDROID_TOOLCHAIN=clang \
#    -DANDROID_STL=c++_shared \
#    -DCMAKE_BUILD_TYPE=Debug \
#    -DCMAKE_ANDROID_ARCH_ABI="armeabi-v7a" \
#    -DANDROID_NDK=${NDK} \
#    -DCMAKE_ANDROID_NDK=${NDK} \
#    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
#    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=./obj/armeabi-v7a \
#    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=./obj/armeabi-v7a \
#    -DCMAKE_MAKE_PROGRAM=${HOME}/Android/Sdk/cmake/3.10.2.4988404/bin/ninja \
#    -DCMAKE_SYSTEM_NAME=Android \
#    ..
    #-DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9 \
    #-DCMAKE_FIND_ROOT_PATH= \
    #-DCMAKE_SYSTEM_VERSION=23 \
