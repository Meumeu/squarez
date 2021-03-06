find_package(Boost REQUIRED COMPONENTS program_options coroutine system context filesystem thread)
find_package(Threads)
find_package(MySQLConnectorCPP)

if (ENABLE_SYSTEMD)
	pkg_check_modules(systemd REQUIRED libsystemd-daemon)
	include_directories(SYSTEM ${systemd_INCLUDE_DIRS})
else()
	add_definitions(-DDISABLE_SYSTEMD)
endif()

add_definitions(-DNO_HTTP_REQUEST)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -DNDEBUG")

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${MYSQLCONNECTORCPP_INCLUDE_DIRS})
add_executable(squarezd server.cpp requesthandler.cpp serverrules.cpp highscores.cpp ${shared_sources})
target_link_libraries(squarezd ${Boost_LIBRARIES} ${MYSQLCONNECTORCPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

if (ENABLE_SYSTEMD)
	target_link_libraries(squarezd ${systemd_LIBRARIES})
endif()

install(TARGETS squarezd DESTINATION bin COMPONENT server)
if (ENABLE_SYSTEMD)
	install(CODE "configure_file(${CMAKE_CURRENT_SOURCE_DIR}/squarezd.service.in ${CMAKE_CURRENT_BINARY_DIR}/squarezd.service)" COMPONENT server)
	install(FILES squarezd.conf DESTINATION /etc/squarezd PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ COMPONENT server)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/squarezd.service squarezd.socket DESTINATION /lib/systemd/system COMPONENT server)
	install(FILES sysusers-squarezd.conf RENAME squarezd.conf DESTINATION /usr/lib/sysusers.d COMPONENT server)
endif()
