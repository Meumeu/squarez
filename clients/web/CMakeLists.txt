configure_file(manifest.appcache.in html/manifest.appcache)
configure_file(manifest.webapp.in html/manifest.webapp)

list(REMOVE_ITEM client_sources ${CMAKE_SOURCE_DIR}/clients/httprequest.cpp)
list(REMOVE_ITEM shared_sources ${CMAKE_SOURCE_DIR}/shared/database/database.cpp)

add_definitions(-Wno-warn-absolute-paths --bind)
add_executable(webclient rulesproxy.cpp cellproxy.cpp eventhandler.cpp httprequest.cpp ${shared_sources} ${client_sources})
target_link_libraries(webclient --bind)
add_dependencies(webclient shapes)

install(TARGETS webclient DESTINATION . COMPONENT html)

install(FILES
	${CMAKE_BINARY_DIR}/data/img/shape0.svg
	${CMAKE_BINARY_DIR}/data/img/shape1.svg
	${CMAKE_BINARY_DIR}/data/img/shape2.svg
	${CMAKE_BINARY_DIR}/data/img/shape0-selected.svg
	${CMAKE_BINARY_DIR}/data/img/shape1-selected.svg
	${CMAKE_BINARY_DIR}/data/img/shape2-selected.svg
	DESTINATION img COMPONENT html)

install(FILES game.css gameSelection.css index.html singleplayer.html tutorial.html DESTINATION . COMPONENT html)
