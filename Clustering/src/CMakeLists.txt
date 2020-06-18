set(BINARY ${CMAKE_PROJECT_NAME})

set(PathTohighguiSo "/usr/local/lib/libopencv_highgui.so") # edit this line, put the path to the "libopencv_highgui.so" file here;
find_package( OpenCV REQUIRED ) # locate OpenCV in system
include_directories( ${OpenCV_INCLUDE_DIRS} ) # provide library headers

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})
# 
# add_executable("../${BINARY}_run" ${SOURCES})
# target_link_libraries("../${BINARY}_run" ${OpenCV_LIBS} ${PathTohighguiSo} )
# add_executable("../${BINARY}_run"  main.cpp)
# 
add_library(src_lib STATIC ${SOURCES})
