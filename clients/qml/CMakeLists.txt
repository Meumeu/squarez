set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)

pkg_check_modules(libcurl REQUIRED libcurl)

QT5_ADD_RESOURCES(resource_sources ${CMAKE_BINARY_DIR}/data/generated_resources.qrc ${CMAKE_SOURCE_DIR}/data/resources.qrc)

add_definitions(-DSQUAREZ_QT)
add_executable(squarezqt main.cpp rulesproxy.cpp cellproxy.cpp selectionproxy.cpp highscores.cpp ${shared_sources} ${client_sources} ${resource_sources})
set_target_properties(squarezqt PROPERTIES AUTOMOC ON)
target_link_libraries(squarezqt Qt5::Core Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Svg ${libcurl_LIBRARIES} ${sqlite_LIBRARIES})

add_dependencies(squarezqt shapes)

install(TARGETS squarezqt DESTINATION bin COMPONENT qt-client)
