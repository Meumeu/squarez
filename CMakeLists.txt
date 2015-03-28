cmake_minimum_required(VERSION 2.6)
project(squarez)

add_definitions(-std=c++11 -g -Wall -Wextra -Werror)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/shared)

set(shared_sources
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/score.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/cell.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/gameboard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/highscores.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/selection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/transition.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/rules/timer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/rules/rules.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/rules/singleplayerrules.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/utils/serializer.cpp)


add_subdirectory(data)
add_subdirectory(clients/qml)
# add_subdirectory(server)
add_subdirectory(test)
