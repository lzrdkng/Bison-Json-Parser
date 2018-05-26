#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([C-JSON], [1.0.0], [olivier.dion@polymtl.ca])
AC_PREREQ([2.69])

AC_CONFIG_SRCDIR([src/lexer.c])
AC_CONFIG_HEADERS([config.h])
# Checks for libraries.
AC_CHECK_LIB([JSON], [JSON_MallocDict])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h])

# Checks for programs.
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_CC_C_O
AC_PROG_YACC


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_REALLOC
AC_CHECK_FUNCS([getdelim strdup])

AM_INIT_AUTOMAKE
AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
                 ])

AC_ENABLE_SHARED
AC_DISABLE_STATIC
LT_INIT

AM_SILENT_RULES([yes])

AC_OUTPUT
