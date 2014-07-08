AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/src -DDATADIR=\"$(datadir)\"

shared_sources = src/shared/board/transition.cpp src/shared/board/gameboard.cpp src/shared/board/selection.cpp src/shared/board/timer.cpp src/shared/serializer.cpp src/shared/network/methods.cpp src/shared/network/urltools.cpp src/shared/board/score.cpp

client_sources = src/client/httprequest.cpp src/shared/rules/rules.cpp src/client/multiplayerrules.cpp src/shared/rules/singleplayerrules.cpp src/client/tutorialrules.cpp src/client/ui.cpp src/client/highscores.cpp

if JAVASCRIPT
squarez_js_SOURCES =  $(shared_sources) $(client_sources) src/js/binding.cpp
squarez_js_LDFLAGS = --bind
#if RELEASE
#squarez_js_LDFLAGS += --closure 1 --llvm-lto 3
#else
#squarez_js_LDFLAGS += --closure 0 --llvm-lto 0
#endif

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
squarezd_SOURCES = $(shared_sources) src/server/server.cpp src/server/requesthandler.cpp src/server/multiplayer/gamestatus.cpp src/server/multiplayer/player.cpp
squarezd_CPPFLAGS = $(AM_CPPFLAGS) $(SYSTEMD_CFLAGS) $(SERVER_CFLAGS) $(BOOST_CPPFLAGS)
squarezd_LDFLAGS = $(SERVER_LIBS) $(SYSTEMD_LIBS) $(BOOST_PROGRAM_OPTIONS_LIBS)
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
