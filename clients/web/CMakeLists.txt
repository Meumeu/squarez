configure_file(manifest.webapp.in manifest.webapp)
configure_file(manifest.appcache.in manifest.appcache.in @ONLY)

list(REMOVE_ITEM client_sources ${CMAKE_SOURCE_DIR}/clients/httprequest.cpp)
list(REMOVE_ITEM shared_sources ${CMAKE_SOURCE_DIR}/shared/database/database.cpp)

add_definitions(-Wno-warn-absolute-paths --bind)
add_executable(webclient rulesproxy.cpp cellproxy.cpp selectionproxy.cpp eventhandler.cpp httprequest.cpp ${shared_sources} ${client_sources})
target_link_libraries(webclient --bind)
add_dependencies(webclient shapes)

install(TARGETS webclient DESTINATION . PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ COMPONENT html)

install(FILES
	${CMAKE_BINARY_DIR}/data/img/shape0.svg
	${CMAKE_BINARY_DIR}/data/img/shape1.svg
	${CMAKE_BINARY_DIR}/data/img/shape2.svg
	${CMAKE_BINARY_DIR}/data/img/shape0-selected.svg
	${CMAKE_BINARY_DIR}/data/img/shape1-selected.svg
	${CMAKE_BINARY_DIR}/data/img/shape2-selected.svg
	${CMAKE_SOURCE_DIR}/data/img/icon30.png
	${CMAKE_SOURCE_DIR}/data/img/icon60.png
	${CMAKE_SOURCE_DIR}/data/img/icon64.png
	${CMAKE_SOURCE_DIR}/data/img/icon90.png
	${CMAKE_SOURCE_DIR}/data/img/icon120.png
	${CMAKE_SOURCE_DIR}/data/img/icon128.png
	DESTINATION img COMPONENT html)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/manifest.webapp
	game.css
	gameSelection.css
	index.html
	singleplayer.html
	tutorial.html
	DESTINATION . COMPONENT html)

install(CODE "
	string(TIMESTAMP MANIFEST_TIMESTAMP \"%Y-%m-%dT%H:%M:%SZ\" UTC)
	file(GLOB_RECURSE FILE_LIST RELATIVE \${CMAKE_INSTALL_PREFIX} \${CMAKE_INSTALL_PREFIX}/*)
	string(REPLACE \";\" \"\n\" FILE_LIST \"\${FILE_LIST}\")
	configure_file(${CMAKE_CURRENT_BINARY_DIR}/manifest.appcache.in ${CMAKE_CURRENT_BINARY_DIR}/manifest.appcache)

	foreach(i \${FILE_LIST})
		message(\${i})
	endforeach()
")

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/manifest.appcache
	DESTINATION . COMPONENT html)
