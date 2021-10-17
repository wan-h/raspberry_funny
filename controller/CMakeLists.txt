set (PROJECT_NAME "controller")
project(controller)

set (SOURCE_PATH ${${PROJECT_NAME}_SOURCE_DIR}/src)
set (INCLUDE_PATH ${${PROJECT_NAME}_SOURCE_DIR}/include)

file(GLOB ALL_SOURCES ${SOURCE_PATH}/*.cpp)

add_library(controller ${ALL_SOURCES})
target_include_directories(controller PUBLIC ${INCLUDE_PATH})
target_link_libraries(controller wiringPi)