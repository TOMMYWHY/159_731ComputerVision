cmake_minimum_required(VERSION 3.14)
project(gesture)

# 添加OpenCV库
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

include_directories(include/)
file(GLOB PROJECT_HEADERS include/*.h)


source_group("Headers" FILES ${PROJECT_HEADERS})


#set(SOURCE_FILES src/demo.cpp src/LoadData.cpp include/LoadData.h)
set(SOURCE_FILES src/main.cpp src/Sample.cpp include/Sample.h)
#set(SOURCE_FILES src/test.cpp)




add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PROJECT_HEADERS} )
target_link_libraries(gesture ${OpenCV_LIBS})

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)



