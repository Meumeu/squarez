#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Squarez], [2.1.0], [xytovl@laposte.net])
AC_CONFIG_SRCDIR([src/shared/gameboard.cpp])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([src/config.h])
AM_SILENT_RULES([yes])

AC_CONFIG_MACRO_DIR([m4])

AC_MSG_CHECKING([whether to build javascript library])
AC_ARG_ENABLE([javascript],
	AS_HELP_STRING([--enable-javascript], [build javascript library]))
AC_MSG_RESULT($enable_javascript)
AM_CONDITIONAL([JAVASCRIPT], [test "x$enable_javascript" = "xyes"])

AC_MSG_CHECKING([whether to build test programs])
AC_ARG_ENABLE([tests],
	AS_HELP_STRING([--enable-tests], [build test programs]))
AC_MSG_RESULT($enable_tests)
AM_CONDITIONAL([TESTS], [test "x$enable_tests" = "xyes"])

AC_ARG_VAR([wwwdir], [http server files directory])

AC_MSG_CHECKING([whether to build server binary])
AC_ARG_ENABLE([server],
	AS_HELP_STRING([--enable-server], [build server library]))
AC_MSG_RESULT($enable_server)
AM_CONDITIONAL([SERVER], [test "x$enable_server" = "xyes"])

AC_MSG_CHECKING([whether to build SDL client])
AC_ARG_ENABLE([sdlclient],
	AS_HELP_STRING([--enable-sdlclient], [build SDL client]))
AC_MSG_RESULT($enable_sdlclient)
AM_CONDITIONAL([SDLCLIENT], [test "x$enable_sdlclient" = "xyes"])

AC_MSG_CHECKING([whether ths SDL client uses SDL2 or SDL1.2])
AC_ARG_WITH([sdl2],
	AS_HELP_STRING([--with-sdl2], [use SDL2]))
AC_MSG_RESULT($with_sdl2)
AM_CONDITIONAL([SDL2], [test "x$with_sdl2" = "xyes"])

AC_MSG_CHECKING([whether to enable server socket activation (systemd)])
AC_ARG_ENABLE([systemd],
	AS_HELP_STRING([--enable-systemd], [enable socket activation]))
AC_MSG_RESULT($enable_systemd)
AM_CONDITIONAL([SYSTEMD], [test "x$enable_systemd" = "xyes"])

AC_MSG_CHECKING([whether to build with -Werror])
AC_ARG_ENABLE([werror],
	AS_HELP_STRING([--enable-werror], [treat warnings as errors]))
AC_MSG_RESULT($enable_werror)
AM_CONDITIONAL([WERROR], [test "x$enable_werror" = "xyes"])

AM_INIT_AUTOMAKE([subdir-objects])

AS_IF([test "x$enable_javascript" = "xyes"], [
	CXXFLAGS=-O2
	AC_CONFIG_FILES([manifest.webapp:src/web/manifest.webapp.in]
		[manifest.appcache:src/web/manifest.appcache.in])
	AS_IF([test "x$wwwdir" = "x"], [
		AC_MSG_ERROR("wwwdir must be set")])
	])

AC_PROG_CXX

# Check C++11
AX_CXX_COMPILE_STDCXX_11

# Libtool
LT_PREREQ([2.4.2])
LT_INIT

CFLAGS="$CFLAGS -Wall -Wextra"
CXXFLAGS="$CXXFLAGS -Wall -Wextra"
CPPFLAGS=""

AS_IF([test "x$enable_werror" = "xyes"], [
	CFLAGS="$CFLAGS -Werror"
	CXXFLAGS="$CXXFLAGS -Werror"
])

# FastCGI library
AS_IF([test "x$enable_server" = "xyes"], [
	PKG_CHECK_MODULES([SERVER], [fastcgi++])
	BOOST_REQUIRE([1.49.0])
	BOOST_PROGRAM_OPTIONS]
)

# SDL2 and OpenGL
AS_IF([test "x$enable_sdlclient" = "xyes"], [
	AS_IF([test "x$with_sdl2" = "xyes"],
	[
		PKG_CHECK_MODULES([SDL2], [sdl2])
	],
	[
		PKG_CHECK_MODULES([SDL], [sdl])
	])
	PKG_CHECK_MODULES([GLES], glesv2)
	BOOST_REQUIRE([])]
)

# Curl
AS_IF([test "x$enable_sdlcient" = "xyes" -o "x$enable_tests" = "xyes"], [
	PKG_CHECK_MODULES([CURL], libcurl)]
)

# Systemd
AS_IF([test "x$enable_systemd" = "xyes"], [
	PKG_CHECK_MODULES([SYSTEMD], [libsystemd-daemon])],
	[CPPFLAGS+="-DDISABLE_SYSTEMD"]
)

AC_OUTPUT(Makefile)
