AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/src -DDATADIR=\"$(datadir)\"

shared_sources = src/shared/transition.cpp src/shared/gameboard.cpp src/shared/selection.cpp src/shared/timer.cpp src/shared/serializer.cpp src/shared/network/methods.cpp src/shared/network/urltools.cpp src/shared/score.cpp

client_sources = src/client/httprequest.cpp src/client/rules.cpp src/client/multiplayerrules.cpp src/client/singleplayerrules.cpp src/client/tutorialrules.cpp src/client/ui.cpp src/client/highscores.cpp

if JAVASCRIPT
squarez_js_SOURCES =  $(shared_sources) $(client_sources) src/js/binding.cpp
squarez_js_LDFLAGS = --bind
if RELEASE
squarez_js_LDFLAGS += --closure 1 --llvm-lto 1
else
squarez_js_LDFLAGS += --closure 0 --llvm-lto 0
endif

www_scriptdir = $(wwwdir)/script
www_script_PROGRAMS = squarez.js
www_script_DATA = src/js/squarezUI.js src/js/singleplayer.js src/js/multiplayer.js src/js/tutorial.js

www_imgdir = $(wwwdir)/img
www_img_DATA = data/img/icon30.png data/img/icon60.png data/img/icon64.png data/img/icon90.png data/img/icon120.png data/img/icon128.png shape0.svg shape1.svg shape2.svg shape0-selected.svg shape1-selected.svg shape2-selected.svg

%.svg: data/img/%.m4 data/img/shape.svg.m4
	m4 -Dm4_config_file=$< $(srcdir)/data/img/shape.svg.m4 > $@
%-selected.svg: data/img/%.m4 data/img/shape.svg.m4
	m4 -Dm4_config_file=$< -Dselected $(srcdir)/data/img/shape.svg.m4 > $@

www_DATA = src/web/singleplayer.html src/web/multiplayer.html src/web/tutorial.html src/web/index.html manifest.webapp manifest.appcache src/web/game.css src/web/gameSelection.css

install-data-hook:
	cd $(DESTDIR)$(wwwdir) && (date "+# %s" && echo "CACHE:" && find * -type f && echo -e "NETWORK:\n*") | grep -v manifest >> manifest.appcache
else

bin_PROGRAMS =

if SERVER
bin_PROGRAMS += squarezd
squarezd_SOURCES = $(shared_sources) src/server/server.cpp src/server/requesthandler.cpp src/server/gamestatus.cpp src/server/player.cpp
squarezd_CPPFLAGS = $(AM_CPPFLAGS) $(SYSTEMD_CFLAGS) $(SERVER_CFLAGS) $(BOOST_CPPFLAGS)
squarezd_LDFLAGS = $(SERVER_LIBS) $(SYSTEMD_LIBS) $(BOOST_PROGRAM_OPTIONS_LIBS)
endif

if SDLCLIENT
bin_PROGRAMS += squarez
squarez_SOURCES = $(shared_sources) $(client_sources) src/sdl/main.cpp src/sdl/sdlui.cpp src/sdl/window.cpp src/sdl/shader.cpp src/sdl/animatable.cpp \
	src/sdl/cell.cpp src/sdl/colour.cpp src/sdl/demo.cpp
squarez_CPPFLAGS = $(AM_CPPFLAGS) $(GLES_CFLAGS) $(CURL_CFLAGS) -DGL_GLEXT_PROTOTYPES=1
squarez_LDFLAGS = $(SDL2_LIBS) $(GLES_LIBS) $(CURL_LIBS)
if SDL2
squarez_CPPFLAGS += $(SDL2_CFLAGS) -DUSE_SDL2
squarez_LDFLAGS += $(SDL2_LIBS)
else
squarez_CPPFLAGS += $(SDL_CFLAGS)
squarez_LDFLAGS += $(SDL_LIBS)
endif
squarezdir = $(datadir)/squarez
squarez_DATA = src/sdl/roundrect.frag.glsl src/sdl/roundrect.vert.glsl
endif

if TESTS
bin_PROGRAMS += squareztest squareztest_bot
squareztest_SOURCES = src/test/main.cpp $(shared_sources) $(client_sources)
squareztest_CPPFLAGS = $(AM_CPPFLAGS) $(CURL_CFLAGS)
squareztest_LDFLAGS = $(CURL_LIBS)
squareztest_bot_SOURCES = src/test/bot.cpp $(shared_sources)
squareztest_bot_CPPFLAGS = $(AM_CPPFLAGS) $(CURL_CFLAGS)
squareztest_bot_LDFLAGS = $(CURL_LIBS)
endif

endif
