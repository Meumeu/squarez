AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/src

shared_sources = src/shared/transition.cpp src/shared/gameboard.cpp src/shared/selection.cpp

if JAVASCRIPT
www_PROGRAMS = squarez.js
squarez_js_SOURCES =  $(shared_sources) src/js/binding.cpp src/shared/timer.cpp
squarez_js_LDFLAGS = --bind --closure 1 --llvm-lto 1
www_DATA = src/js/game.js src/js/singleplayer.js src/js/multiplayer.js src/web/gameSelection.css src/web/game.css src/web/singleplayer.html src/web/multiplayer.html src/web/index.html
else

bin_PROGRAMS = squareztest squareztest_bot

if SERVER
bin_PROGRAMS += squarezd
squarezd_SOURCES = $(shared_sources) src/server/server.cpp src/server/requesthandler.cpp src/server/gamestatus.cpp src/server/player.cpp
squarezd_CPPFLAGS = $(AM_CPPFLAGS) $(SYSTEMD_CFLAGS) $(SERVER_CFLAGS) $(BOOST_CPPFLAGS)
squarezd_LDFLAGS = $(SERVER_LIBS) $(SYSTEMD_LIBS) $(BOOST_PROGRAM_OPTIONS_LIBS)
endif

if SDLCLIENT
bin_PROGRAMS += squarezsdl
squarezsdl_SOURCES = $(shared_sources) src/sdl/main.cpp src/sdl/game.cpp src/sdl/window.cpp src/sdl/shader.cpp src/sdl/animatable.cpp src/sdl/cell.cpp src/sdl/colour.cpp src/sdl/gamerules.cpp
squarezsdl_CPPFLAGS = $(AM_CPPFLAGS) $(SDL2_CFLAGS) $(GL_CFLAGS) -DSDL_MAIN_HANDLED -DGL_GLEXT_PROTOTYPES=1
squarezsdl_LDFLAGS = $(SDL2_LIBS) $(GL_LIBS)
endif

squareztest_SOURCES = src/test/main.cpp $(shared_sources)

squareztest_bot_SOURCES = src/test/bot.cpp $(shared_sources)

endif
