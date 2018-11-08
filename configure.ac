#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([maze], [1.0], [aiden.woodruff@gmail.com])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_CPP

# Checks for libraries.
AC_CHECK_LIB([ncurses], [initscr], [], [AC_MSG_ERROR([ncurses not found]) ] )

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strtol])

AC_CONFIG_FILES([test/Makefile Makefile test/sanity test/blank.sh test/put.sh])
AM_INIT_AUTOMAKE
AC_OUTPUT
chmod +x test/sanity test/blank.sh test/put.sh
