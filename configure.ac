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

AC_PREREQ(2.52)
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
# FIXME: Replace `main' with a function in `-lGL':
AC_CHECK_LIB([GL], [main])
# FIXME: Replace `main' with a function in `-lGLU':
AC_CHECK_LIB([GLU], [main])
# FIXME: Replace `main' with a function in `-lcurl':
AC_CHECK_LIB([curl], [main])
# FIXME: Replace `main' with a function in `-lfltk':
AC_CHECK_LIB([fltk], [main])
# FIXME: Replace `main' with a function in `-lfltk_gl':
AC_CHECK_LIB([fltk_gl], [main])
# FIXME: Replace `main' with a function in `-lfltk_images':
AC_CHECK_LIB([fltk_images], [main])
# FIXME: Replace `main' with a function in `-losg':
AC_CHECK_LIB([osg], [main])
# FIXME: Replace `main' with a function in `-losgViewer':
AC_CHECK_LIB([osgViewer], [main])

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
