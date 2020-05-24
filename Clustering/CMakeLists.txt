cmake_minimum_required(VERSION 3.9)
project(clustering)

set(CMAKE_CXX_STANDARD 11)
set(PathTohighguiSo "/usr/local/lib/libopencv_highgui.so") # edit this line, put the path to the "libopencv_highgui.so" file here;
find_package( OpenCV REQUIRED ) # locate OpenCV in system
include_directories( ${OpenCV_INCLUDE_DIRS} ) # provide library headers

add_executable(clustering main.cpp)
# target_link_libraries( clustering ${OpenCV_LIBS} /usr/lib/x86_64-linux-gnu/libopencv_highgui.so) # link OpenCV libraries , hightgui.so not found by cmake so this hack
# target_link_libraries( clustering ${OpenCV_LIBS} /usr/local/lib/libopencv_highgui.so) # link OpenCV libraries , hightgui.so not found by cmake so this hack
target_link_libraries( clustering ${OpenCV_LIBS} ${PathTohighguiSo}) # link OpenCV libraries , hightgui.so not found by cmake so this hack

add_executable(klustering bloatMain.cpp)
target_link_libraries( klustering ${OpenCV_LIBS} ${PathTohighguiSo}) # link OpenCV libraries , hightgui.so not found by cmake so this hack

add_executable(test test.cpp)
target_link_libraries( test ${OpenCV_LIBS} )

add_executable(video videoPlayTest.cpp)
target_link_libraries( video ${OpenCV_LIBS} ${PathTohighguiSo}) # link OpenCV libraries , hightgui.so not found by cmake so this hack

add_executable(ffull f3ull.cpp)
target_link_libraries( ffull ${OpenCV_LIBS} ${PathTohighguiSo}) # link OpenCV libraries , hightgui.so not found by cmake so this hack

# add_executable(full full.cpp)
# target_link_libraries( full ${OpenCV_LIBS} ${PathTohighguiSo}) # link OpenCV libraries , hightgui.so not found by cmake so this hack

MESSAGE("OpenCV_LIBS: " ${OpenCV_LIBS} )  #display opencv libs found

