
add_executable(squareztest_bot bot.cpp ${shared_sources})
set_target_properties(squareztest_bot PROPERTIES COMPILE_DEFINITIONS "SQUAREZ_BOT")
