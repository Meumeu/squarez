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
AC_MSG_RESULT($enable_simulator)
AM_CONDITIONAL([JAVASCRIPT], [test "x$enable_javascript" = "xyes"])

AC_MSG_CHECKING([whether to build with -Werror])
AC_ARG_ENABLE([werror],
	AS_HELP_STRING([--enable-werror], [treat warnings as errors]))
AC_MSG_RESULT($enable_werror)
AM_CONDITIONAL([WERROR], [test "x$enable_werror" = "xyes"])

AM_INIT_AUTOMAKE([subdir-objects])

# Checks for programs.
AC_PROG_CXX

# Check C++11 and C99
AX_CXX_COMPILE_STDCXX_11

# Libtool
LT_PREREQ([2.4.2])
LT_INIT

CFLAGS="$CFLAGS -Wall -Wextra"
CXXFLAGS="$CXXFLAGS -Wall -Wextra"

AS_IF([test "x$enable_werror" = "xyes"], [
	CFLAGS="$CFLAGS -Werror"
	CXXFLAGS="$CXXFLAGS -Werror"
])

AC_OUTPUT(Makefile)
