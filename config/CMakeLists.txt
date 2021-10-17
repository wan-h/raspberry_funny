set (PROJECT_NAME "config")
project(config)

add_library(config config.cpp ini.cpp)
target_include_directories(config PUBLIC ${${PROJECT_NAME}_SOURCE_DIR})