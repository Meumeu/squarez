find_package(Boost REQUIRED COMPONENTS program_options)

find_package(PkgConfig)
pkg_check_modules(fastcgipp REQUIRED fastcgi++)

if (ENABLE_SYSTEMD)
	pkg_check_modules(systemd REQUIRED libsystemd-daemon)
	include_directories(SYSTEM ${systemd_INCLUDE_DIRS})
else()
	add_definitions(-DDISABLE_SYSTEMD)
endif()

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${fastcgipp_INCLUDE_DIRS})
add_executable(squarezd server.cpp requesthandler.cpp ${shared_sources})
target_link_libraries(squarezd ${Boost_LIBRARIES} ${fastcgipp_LIBRARIES})

if (ENABLE_SYSTEMD)
	target_link_libraries(squarezd ${systemd_LIBRARIES})
endif()