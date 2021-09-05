add_library(camera camera.cpp)

# 表示任何需要链接到该库的目标需要包含该资源目录找到 .h 文件
target_include_directories(camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS}) 
target_link_libraries(camera ${OpenCV_LIBS})