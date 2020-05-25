cmake_minimum_required(VERSION 3.9)
project(clustering)

set(CMAKE_CXX_STANDARD 11)
set(PathTohighguiSo "/usr/local/lib/libopencv_highgui.so") # edit this line, put the path to the "libopencv_highgui.so" file here;
find_package( OpenCV REQUIRED ) # locate OpenCV in system
include_directories( ${OpenCV_INCLUDE_DIRS} ) # provide library headers

add_executable(image image.cpp)
target_link_libraries( image ${OpenCV_LIBS} ${PathTohighguiSo}) # link OpenCV libraries , hightgui.so not found by cmake so this hack

add_executable(test test.cpp)
target_link_libraries( test ${OpenCV_LIBS} )

add_executable(videoTest videoPlayTest.cpp)
target_link_libraries( videoTest ${OpenCV_LIBS} ${PathTohighguiSo}) # link OpenCV libraries , hightgui.so not found by cmake so this hack

add_executable(video video.cpp)
target_link_libraries( video ${OpenCV_LIBS} ${PathTohighguiSo}) # link OpenCV libraries , hightgui.so not found by cmake so this hack

# add_executable(full full.cpp)
# target_link_libraries( full ${OpenCV_LIBS} ${PathTohighguiSo}) # link OpenCV libraries , hightgui.so not found by cmake so this hack

MESSAGE("OpenCV_LIBS: " ${OpenCV_LIBS} )  #display opencv libs found

