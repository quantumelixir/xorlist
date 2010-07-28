project(xorlist)

cmake_minimum_required(VERSION 2.6)

include_directories(include)
add_subdirectory(src)

# make demo
add_executable(demo demo.c)
target_link_libraries(demo xorlist)
