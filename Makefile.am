AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/src

bin_PROGRAMS=squareztest squarez.js

squareztest_SOURCES = src/test/main.cpp src/shared/transition.cpp src/shared/gameboard.cpp

squarez_js_SOURCES = src/shared/transition.cpp src/shared/gameboard.cpp src/js/binding.cpp
squarez_js_LDFLAGS = --bind
