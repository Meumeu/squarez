cmake_minimum_required(VERSION 3.0)
project(squarez VERSION 3.0)

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

set(ENABLE_CLOSURE ON CACHE BOOL "Use the closure compiler for the web client")

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
set(ENABLE_SYSTEMD ON CACHE BOOL "Enable systemd socket activation")
else()
set(ENABLE_SYSTEMD OFF CACHE BOOL "Enable systemd socket activation" FORCE)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(sqlite REQUIRED sqlite3)

add_definitions(-std=c++11 -g -Wall -Wextra -Werror -DPACKAGE="squarez" -DPACKAGE_VERSION="${PROJECT_VERSION}")
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/shared ${CMAKE_CURRENT_SOURCE_DIR}/clients)

include(CheckIncludeFileCXX)

check_include_file_cxx(cxxabi.h HAVE_CXXABI_H)
check_include_file_cxx(execinfo.h HAVE_EXECINFO_H)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shared/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)


set(shared_sources
	${CMAKE_CURRENT_SOURCE_DIR}/shared/database/database.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/score.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/cell.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/constants.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/gameboard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/selection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/game/transition.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/network/methods.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/network/urltools.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/rules/timer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/rules/rules.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/rules/singleplayerrules.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/utils/serializer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shared/utils/unicode.cpp)

set(client_sources
	${CMAKE_CURRENT_SOURCE_DIR}/clients/tutorialrules.cpp
)

set(CPACK_COMPONENTS_ALL)

if(BUILD_QML_CLIENT OR BUILD_WEB_CLIENT)
add_subdirectory(data)
endif()

if(BUILD_QML_CLIENT)
add_subdirectory(clients/qml)
set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} qt-client)
endif()

if(BUILD_WEB_CLIENT)
add_subdirectory(clients/web)
set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} html)
endif()

if(BUILD_SERVER)
add_subdirectory(server)
set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} server)
endif()

if(BUILD_TEST)
add_subdirectory(test)
endif()

set(CPACK_PACKAGE_NAME "squarez")
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${PROJECT_VERSION})

set(CPACK_TGZ_COMPONENT_INSTALL ON)
set(CPACK_DEB_COMPONENT_INSTALL ON)

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "squarez puzzle game")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Squarez puzzle game
 .
 This is a remake of the Squarez game by Eric Burgess
 .
 You are presented a 8x8 grid of 3 different symbols and a timer,
 Selecting 4 identical elements forming the edges of the square
 remove the edges, refill the timer and give you points. The bigger
 the square is, the more time and points you will get. Squares that
 are not horizontal or 45° have bonus point and time.")
set(CPACK_PACKAGE_CONTACT "Guillaume Meunier <>")

if(${CMAKE_SYSTEM_NAME} STREQUAL Emscripten)
	set(CPACK_STRIP_FILES OFF)
else()
	set(CPACK_STRIP_FILES ON)
endif()
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_DEBIAN_PACKAGE_SECTION "games")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_SET_DESTDIR ON)
set(CPACK_COMPONENTS_IGNORE_GROUPS 1)
include(CPack)
