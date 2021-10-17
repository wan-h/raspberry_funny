project(detector)

add_library(detector UltraFace.cpp)
target_include_directories(detector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${NCNN_INSTALL}/include/ncnn ${OpenCV_INCLUDE_DIRS}) 
target_link_directories(detector PUBLIC ${NCNN_INSTALL}/lib)
target_link_libraries(detector ncnn ${OpenCV_LIBS})