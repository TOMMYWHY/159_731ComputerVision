cmake_minimum_required(VERSION 3.14)
project(kuwahara)

# 添加OpenCV库
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

include_directories(include/)
source_group("Headers" FILES ${PROJECT_HEADERS})

#target_link_libraries(Video ${OpenCV_LIBS}) # 链接库



#set(SOURCE_FILES src/main.cpp)

set(SOURCE_FILES src/main-Y.cpp)



add_executable(kuwahara ${SOURCE_FILES} ${PROJECT_HEADERS} )
target_link_libraries(kuwahara ${OpenCV_LIBS})



