pkg_check_modules(libcurl REQUIRED libcurl)

add_definitions(-DNO_HTTP_REQUEST)
add_executable(squareztest_bot bot.cpp ${shared_sources})
set_target_properties(squareztest_bot PROPERTIES COMPILE_DEFINITIONS "SQUAREZ_BOT")

target_link_libraries(squareztest_bot ${libcurl_LIBRARIES})
