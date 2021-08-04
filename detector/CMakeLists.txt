add_library(detector detector.cpp)

# 表示任何需要链接到该库的目标需要包含该资源目录找到 .h 文件
target_include_directories(detector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${NCNN_INSTALL}/include/ncnn ${OpenCV_INCLUDE_DIRS}) 
target_link_libraries(detector ncnn ${OpenCV_LIBS})