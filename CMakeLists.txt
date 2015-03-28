cmake_minimum_required(VERSION 2.6)
project(squarez)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)

add_definitions(-std=c++11 -g -Wall -Wextra -Werror)
#add_definitions(-std=c++11 -g -Wall -Wextra)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
#include_directories(${Qt5Core_INCLUDES} ${Qt5Qml_INCLUDES} ${Qt5Quick_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
#add_definitions(${Qt5Core_DEFINITIONS} ${Qt5Qml_DEFINITIONS} ${Qt5Quick_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
#set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

configure_file(generated_resources.qrc generated_resources.qrc COPYONLY)
QT5_ADD_RESOURCES(ResourceSources ${CMAKE_CURRENT_BINARY_DIR}/generated_resources.qrc resources.qrc)

set(shared_sources src/game/cell.cpp src/game/gameboard.cpp src/game/highscores.cpp src/game/score.cpp src/game/selection.cpp src/game/transition.cpp src/utils/serializer.cpp)

set(client_sources src/rules/rules.cpp src/rules/singleplayerrules.cpp src/rules/timer.cpp)

set(qt_sources src/proxy/qt/rulesproxy.cpp src/proxy/qt/cellproxy.cpp)


add_executable(squareztest_bot src/test/bot.cpp ${shared_sources} ${client_sources})
set_target_properties(squareztest_bot PROPERTIES COMPILE_DEFINITIONS "PACKAGE=\"squarez\";SQUAREZ_BOT")


add_executable(squarezqt src/test/testqt.cpp ${shared_sources} ${client_sources} ${qt_sources} ${ResourceSources})
set_target_properties(squarezqt PROPERTIES AUTOMOC ON)
set_target_properties(squarezqt PROPERTIES COMPILE_DEFINITIONS "PACKAGE=\"squarez\";SQUAREZ_QT")
target_link_libraries(squarezqt Qt5::Core Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Svg)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/img)
foreach(i shape0 shape1 shape2)
	add_custom_command(OUTPUT img/${i}.svg
		COMMAND m4 -Dm4_config_file=${i}.m4 shape.svg.m4 > ${CMAKE_CURRENT_BINARY_DIR}/img/${i}.svg
		DEPENDS data/img/${i}.m4 data/img/shape.svg.m4
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/img)
	add_custom_command(OUTPUT img/${i}-selected.svg
		COMMAND m4 -Dm4_config_file=${i}.m4 -Dselected shape.svg.m4 > ${CMAKE_CURRENT_BINARY_DIR}/img/${i}-selected.svg
		DEPENDS data/img/${i}.m4 data/img/shape.svg.m4
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/img)
endforeach()

add_custom_target(shapes ALL DEPENDS img/shape0.svg img/shape1.svg img/shape2.svg img/shape0-selected.svg img/shape1-selected.svg img/shape2-selected.svg)

install(TARGETS squareztest_bot RUNTIME DESTINATION bin)
install(TARGETS squarezqt RUNTIME DESTINATION bin)
add_dependencies(squarezqt shapes)