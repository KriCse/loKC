cmake_minimum_required(VERSION 3.14)
project(lokC C)

set(CMAKE_C_STANDARD 11)
find_package(X11 REQUIRED)
add_executable(lokC lokc.c lock.h)
include_directories(${X11_INCLUDE_DIR})
link_directories(${X11_LIBRARIES})
target_link_libraries(lokC ${X11_LIBRARIES})
