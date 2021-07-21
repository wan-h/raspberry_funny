cmake_minimum_required(VERSION 3.10)
project(detector VERSION 1.0)

add_library(detector detector.cpp)

# 表示任何需要链接到该库的目标需要包含该资源目录找到 .h 文件
target_include_directories(detector 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} 
    PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(detector PRIVATE ncnn ${OpenCV_LIBS})

install(TARGETS detector DESTINATION lib)
install(FILES detector.h DESTINATION include)