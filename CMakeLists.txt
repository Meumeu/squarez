project(squarez)

cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(Boost 1.53.0)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_definitions(-std=c++11)

include_directories(src)

add_executable(squarez src/shared/gameboard.cpp src/shared/main.cpp src/shared/transition.cpp)

install(TARGETS squarez RUNTIME DESTINATION bin)

add_subdirectory(src)