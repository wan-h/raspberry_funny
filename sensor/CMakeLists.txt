set (PROJECT_NAME "sensor")
project(sensor)

set (SOURCE_PATH ${${PROJECT_NAME}_SOURCE_DIR}/src)
set (INCLUDE_PATH ${${PROJECT_NAME}_SOURCE_DIR}/include)

file(GLOB ALL_SOURCES ${SOURCE_PATH}/*.cpp)

add_library(sensor ${ALL_SOURCES})
target_include_directories(sensor PUBLIC ${INCLUDE_PATH} ${OpenCV_INCLUDE_DIRS})
target_link_libraries(sensor wiringPi ${OpenCV_LIBS})