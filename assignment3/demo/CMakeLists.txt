cmake_minimum_required(VERSION 3.14)
project(gesture)

# 添加OpenCV库
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

include_directories(include/)
file(GLOB PROJECT_HEADERS include/*.h)


source_group("Headers" FILES ${PROJECT_HEADERS})


#set(SOURCE_FILES src/main.cpp src/ImgModel.cpp include/ImgModel.h src/demo.cpp)
set(SOURCE_FILES src/demo.cpp)




add_executable(gesture ${SOURCE_FILES} ${PROJECT_HEADERS} )
target_link_libraries(gesture ${OpenCV_LIBS})



