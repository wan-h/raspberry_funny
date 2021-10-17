cmake_minimum_required(VERSION 3.10)

# 设置项目名字和版本
project(raspberry_funny VERSION 1.0)

# 设置ncnn安装位置
set(NCNN_INSTALL "/home/pi/workspace/ncnn/build/install" CACHE INTERNAL "")
message(STATUS "NCNN library: ${NCNN_INSTALL}")

# 调用opencv
find_package(OpenCV REQUIRED)
# 调用openmp
find_package(OpenMP REQUIRED) 
if(OPENMP_FOUND)  
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")  
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")  
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")  
endif()


add_subdirectory(detector)
add_subdirectory(sensor)
add_subdirectory(controller)
add_subdirectory(config)
list(APPEND EXTRA_LIBS sensor controller config detector)

add_executable(funny main.cpp)
target_link_libraries(funny ${EXTRA_LIBS})
