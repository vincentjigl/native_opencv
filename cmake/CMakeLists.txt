cmake_minimum_required(VERSION 3.4.1)
project(teststitch)

include_directories(${OpenCV_DIR}/jni/include)
add_library( lib_opencv SHARED IMPORTED )
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${OpenCV_DIR}/libs/${ANDROID_ABI}/libopencv_java4.so)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.



find_library( # Sets the name of the path variable.
        log-lib
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

add_executable(teststitch ./test/main.cpp ./lib/v4l2cap_img.cpp)
target_link_libraries(teststitch lib_opencv ${log-lib})

add_executable(testyuv ./test/test.cpp)
target_link_libraries(testyuv lib_opencv ${log-lib})
