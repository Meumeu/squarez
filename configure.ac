#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Squarez], [0.1], [xytovl@laposte.net])
AC_CONFIG_SRCDIR([src/shared/gameboard.cpp])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([src/config.h])

AC_CONFIG_MACRO_DIR([m4])

AC_MSG_CHECKING([whether to build javascript library])
AC_ARG_ENABLE([javascript],
	AS_HELP_STRING([--enable-javascript], [build javascript library]))
AC_MSG_RESULT($enable_javascript)
AM_CONDITIONAL([JAVASCRIPT], [test "x$enable_javascript" = "xyes"])

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

# Checks for programs.
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

#FastCGI library
AS_IF([test "x$enable_server" = "xyes"], [
	PKG_CHECK_MODULES([SERVER], [fastcgi++])
	BOOST_REQUIRE([1.53.0])
	BOOST_PROGRAM_OPTIONS]
)

# Systemd
AS_IF([test "x$enable_systemd" = "xyes"], [
	PKG_CHECK_MODULES([SYSTEMD], [libsystemd-daemon])],
	[CPPFLAGS+="-DDISABLE_SYSTEMD"]
)

AC_OUTPUT(Makefile)
