cmake_minimum_required(VERSION 3.9)
project(clustering)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

# set(PathTohighguiSo "/usr/local/lib/libopencv_highgui.so") # edit this line, put the path to the "libopencv_highgui.so" file here;
find_package( OpenCV REQUIRED ) # locate OpenCV in system
include_directories( ${OpenCV_INCLUDE_DIRS} ) # provide library headers

add_subdirectory(src)
file(GLOB Sources "*.cpp")
add_executable(${BINARY}_run ${Sources})
target_link_libraries(${BINARY}_run src_lib ${OpenCV_LIBS} )
# add_executable(${BINARY}_run ${SOURCES})
# target_link_libraries(${BINARY}_run ${OpenCV_LIBS} ${PathTohighguiSo} )
# add_executable(${BINARY}_run main.cpp)


