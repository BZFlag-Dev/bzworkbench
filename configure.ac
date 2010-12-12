# BZWorkbench
# Copyright (c) 1993-2010 Tim Riker
#
# This package is free software;  you can redistribute it and/or
# modify it under the terms of the license found in the file
# named COPYING that should have accompanied this file.
#
# THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

AC_PREREQ([2.52])
AC_INIT([BZWorkbench], [0.1], [http://BZFlag.org])
AM_INIT_AUTOMAKE([1.6 foreign])
AC_CONFIG_SRCDIR([src/main.cpp])
AM_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.
AC_CHECK_LIB([OpenThreads], [OpenThreadsGetVersion])
AC_CHECK_LIB([curl], [curl_easy_init], , AC_MSG_ERROR([curl library is required]))
AC_CHECK_LIB([dl], [dlopen], , AC_MSG_ERROR([dl library is required]))
AC_CHECK_LIB([fltk], [_init], , AC_MSG_ERROR([fltk library is required]))
AC_CHECK_LIB([fltk_gl], [_init])
AC_CHECK_LIB([fltk_images], [_init])

AC_MSG_CHECKING([for FL_Scroll::scroll_to() in FLTK])
# The scroll_to() method was introduced in FLTK 1.3
AC_LANG(C++)
AC_LINK_IFELSE(
 AC_LANG_PROGRAM([[#include <FL/Fl_Scroll.H>]],[[  Fl_Scroll x(0,0,0,0);
  x.scroll_to(0,0);]]),
 [AC_MSG_RESULT(yes) AC_DEFINE([HAVE_SCROLL_TO],[1],[FLTK includes Fl_Scroll::scroll_to])],
 [AC_MSG_RESULT(no)])
AC_LANG(C)

AC_CHECK_LIB([osgDB], [osgDBGetVersion])
AC_CHECK_LIB([osgGA], [osgGAGetVersion])
AC_CHECK_LIB([osgViewer], [osgViewerGetVersion])
AC_CHECK_LIB([osg], [osgGetVersion], , AC_MSG_ERROR([osg library is required]))

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([memory.h stddef.h stdlib.h string.h sys/param.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([memmove memset mkdir realpath sqrt strchr strrchr])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
