set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)

QT5_ADD_RESOURCES(resource_sources ${CMAKE_BINARY_DIR}/data/generated_resources.qrc ${CMAKE_SOURCE_DIR}/data/resources.qrc)

set(qt_sources main.cpp proxy/rulesproxy.cpp proxy/cellproxy.cpp)

add_executable(squarezqt ${shared_sources} ${qt_sources} ${resource_sources})
set_target_properties(squarezqt PROPERTIES AUTOMOC ON)
set_target_properties(squarezqt PROPERTIES COMPILE_DEFINITIONS "SQUAREZ_QT")
target_link_libraries(squarezqt Qt5::Core Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Svg)

add_dependencies(squarezqt shapes)
