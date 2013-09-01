AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/src

bin_PROGRAMS=squareztest squarezjs

squareztest_SOURCES = src/test/main.cpp src/shared/transition.cpp src/shared/gameboard.cpp

squarezjs_SOURCES = src/shared/transition.cpp src/shared/gameboard.cpp src/js/binding.cpp
