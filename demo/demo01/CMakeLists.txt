cmake_minimum_required(VERSION 3.14)
project(opencv_demo)

# 添加OpenCV库
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)


#target_link_libraries(Video ${OpenCV_LIBS}) # 链接库


# self learning
#set(SOURCE_FILES sl/main.cpp)


# week 02 lect
#set(SOURCE_FILES week02/main.cpp)


# week 03 lect
set(SOURCE_FILES week03/main.cpp)

# week 04 lect
#set(SOURCE_FILES week04/main.cpp)



add_executable(opencv_demo ${SOURCE_FILES})
target_link_libraries(opencv_demo ${OpenCV_LIBS}) # 链接库

#add_executable(opencv_01 main.cpp)


