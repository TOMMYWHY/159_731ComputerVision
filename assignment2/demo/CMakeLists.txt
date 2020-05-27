cmake_minimum_required(VERSION 3.14)
project(barcodes)

# 添加OpenCV库
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

include_directories(include/)
file(GLOB PROJECT_HEADERS include/*.h)


source_group("Headers" FILES ${PROJECT_HEADERS})

#target_link_libraries(Video ${OpenCV_LIBS}) # 链接库
#


#set(SOURCE_FILES src/main.cpp)
set(SOURCE_FILES src/demo.cpp src/ImgModel.cpp include/ImgModel.h)




add_executable(barcodes ${SOURCE_FILES} ${PROJECT_HEADERS} )
target_link_libraries(barcodes ${OpenCV_LIBS})



