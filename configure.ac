#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Squarez], [2.99.0], [squarez@meumeu.org])
AC_CONFIG_SRCDIR([src/game/gameboard.cpp])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([src/config.h])
AM_SILENT_RULES([yes])

AC_CONFIG_MACRO_DIR([m4])

AC_ARG_ENABLE([release],
	AS_HELP_STRING([--enable-release], [build in release mode]))
AC_MSG_RESULT($enable_release)
AM_CONDITIONAL([RELEASE], [test "x$enable_release" = "xyes"])

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
	AS_IF([test "x$enable_release" = "xyes"], [
		CXXFLAGS=-Oz])
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
CXXFLAGS="$CXXFLAGS -Wall -Wextra -fno-use-cxa-atexit"
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

# Curl
AS_IF([test "x$enable_tests" = "xyes"], [
	PKG_CHECK_MODULES([CURL], libcurl)]
)

# Systemd
AS_IF([test "x$enable_systemd" = "xyes"], [
	PKG_CHECK_MODULES([SYSTEMD], [libsystemd-daemon])],
	[CPPFLAGS+="-DDISABLE_SYSTEMD"]
)

AC_OUTPUT(Makefile)
