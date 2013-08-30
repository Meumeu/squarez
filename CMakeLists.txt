project(squarez)

cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c++11)

add_executable(squarez src/shared/gameboard.cpp src/shared/main.cpp)

install(TARGETS squarez RUNTIME DESTINATION bin)

add_subdirectory(src)