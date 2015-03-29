cmake_minimum_required(VERSION 2.6)
project(squarez)

if(${CMAKE_SYSTEM_NAME} STREQUAL Emscripten)
set(BUILD_TEST OFF CACHE BOOL "Build the test bot" FORCE)
set(BUILD_QML_CLIENT OFF CACHE BOOL "Build the QML client" FORCE)
set(BUILD_WEB_CLIENT ON CACHE BOOL "Build the Web client")
set(BUILD_SERVER OFF CACHE BOOL "Build the server" FORCE)
else()
set(BUILD_TEST ON CACHE BOOL "Build the test bot")
set(BUILD_QML_CLIENT ON CACHE BOOL "Build the QML client")
set(BUILD_WEB_CLIENT OFF CACHE BOOL "Build the Web client" FORCE)
set(BUILD_SERVER ON CACHE BOOL "Build the server")
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
set(ENABLE_SYSTEMD ON CACHE BOOL "Enable systemd socket activation")
else()
set(ENABLE_SYSTEMD OFF CACHE BOOL "Enable systemd socket activation" FORCE)
endif()

add_definitions(-std=c++11 -g -Wall -Wextra -Werror -DPACKAGE="squarez")
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/shared)

set(shared_sources
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/score.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/cell.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/gameboard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/selection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/transition.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/network/methods.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/network/urltools.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/rules/timer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/rules/rules.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/rules/singleplayerrules.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/utils/serializer.cpp)


if(BUILD_QML_CLIENT OR BUILD_WEB_CLIENT)
add_subdirectory(data)
endif()

if(BUILD_QML_CLIENT)
add_subdirectory(clients/qml)
endif()

if(BUILD_WEB_CLIENT)
add_subdirectory(clients/web)
endif()

if(BUILD_SERVER)
add_subdirectory(server)
endif()

if(BUILD_TEST)
add_subdirectory(test)
endif()
