AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/src

bin_PROGRAMS = squareztest squareztest_bot

shared_sources = src/shared/transition.cpp src/shared/gameboard.cpp src/shared/selection.cpp

if JAVASCRIPT
bin_PROGRAMS += squarez.js
squarez_js_SOURCES =  $(shared_sources) src/js/binding.cpp src/shared/timer.cpp
squarez_js_LDFLAGS = --bind
endif

if SERVER
bin_PROGRAMS += squarezd
squarezd_SOURCES = $(shared_sources) src/server/server.cpp src/server/requesthandler.cpp
squarezd_CPPFLAGS = $(AM_CPPFLAGS) $(SYSTEMD_CFLAGS) $(SERVER_CFLAGS) $(BOOST_CPPFLAGS)
squarezd_LDFLAGS = $(SERVER_LIBS) $(SYSTEMD_LIBS) $(BOOST_PROGRAM_OPTIONS_LIBS)
endif

squareztest_SOURCES = src/test/main.cpp $(shared_sources)

squareztest_bot_SOURCES = src/test/bot.cpp $(shared_sources)
