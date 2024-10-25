dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([u8glib],[1.18],[BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([csrc/u8g_font.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wall -Wno-obsolete -Wno-portability subdir-objects])
AM_MAINTAINER_MODE

#magic for conditional check in Makefile:
MK=''; AC_SUBST([MK])
SED=sed

# Checks for programs.
AC_DEFUN([AC_REQUIRE_PROGS],[
  AC_REQUIRE([AC_PROG_AWK])dnl
  AC_REQUIRE([AC_PROG_EGREP])dnl
  AC_REQUIRE([AC_PROG_MAKE_SET])dnl
  AC_REQUIRE([AC_USE_SYSTEM_EXTENSIONS])dnl
  AC_REQUIRE([AC_PROG_CC])dnl
  m4_ifdef([AM_PROG_CC_C_O],[dnl
    AC_REQUIRE([AM_PROG_CC_C_O])dnl
  ])dnl
  AC_REQUIRE([AC_PROG_RANLIB])dnl
])dnl
AC_REQUIRE_PROGS
AC_PROG_CPP
AC_PROG_CXX
AM_PROG_AR
AM_PROG_AS
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PATH_PROG([FREETYPE_CONFIG],[freetype-config],[false])
AC_PATH_PROG([SDL_CONFIG],[sdl-config],[false])dnl

AC_PATH_PROG([VALGRIND],[valgrind])dnl

AM_CONDITIONAL([HAVE_VALGRIND],[test "x${VALGRIND}" != "x" && test -x "${VALGRIND}"])dnl

# Checks for libraries.
LT_PREREQ([2.2])
LT_INIT([disable-static])dnl

# debug
AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug],
                        [Compile the debug version (default: disabled)])],
	[enable_debug=${enableval}],
	[enable_debug=no])
AM_CONDITIONAL([DEBUG],[test "x${enable_debug}" = "xyes"])
if test "x${enable_debug}" = "xyes"; then
  dnl# Have to do "changequote" because of how sed is called here:
  changequote({,})
  CFLAGS=`echo "${CFLAGS}" | ${SED} -e 's/-O[0-9s]*//g'`
  CXXFLAGS=`echo "${CXXFLAGS}" | ${SED} -e 's/-O[0-9s]*//g'`
  changequote([,])
  dnl# add -O0 only if GCC or ICC is used
  if test "x${GCC}" = "xyes" || test "x${ICC}" = "xyes"; then
    CFLAGS="${CFLAGS} -g -O0 -Wall"
    CXXFLAGS="${CXXFLAGS} -g -O0 -Wall"
  fi
else
  dnl# "changequote" is evil; no need to use it here:
  CFLAGS=`echo "${CFLAGS}" | ${SED} -e 's/-g//g'`
  CXXFLAGS=`echo "${CXXFLAGS}" | ${SED} -e 's/-g//g'`
fi

if test "x${LFS_CFLAGS}" = "x"; then
  test -z "${LFS_CFLAGS}" && export LFS_CFLAGS="`getconf LFS_CFLAGS 2>/dev/null`"
fi
if test "x${LFS64_CFLAGS}" = "x"; then
  test -z "${LFS64_CFLAGS}" && export LFS64_CFLAGS="`getconf LFS64_CFLAGS 2>/dev/null`"
fi
if test "x${LFS_LDFLAGS}" = "x"; then
  test -z "${LFS_LDFLAGS}" && export LFS_LDFLAGS="`getconf LFS_LDFLAGS 2>/dev/null`"
fi
if test "x${LFS64_LDFLAGS}" = "x"; then
  test -z "${LFS64_LDFLAGS}" && export LFS64_LDFLAGS="`getconf LFS64_LDFLAGS 2>/dev/null`"
fi
AC_SUBST([LFS_CFLAGS])dnl
AC_SUBST([LFS64_CFLAGS])dnl
AC_SUBST([LFS_LDFLAGS])dnl
AC_SUBST([LFS64_LDFLAGS])dnl

# Checks for libraries.
AC_CHECK_LIB([c],[strerror])dnl
dnl# FIXME: Replace `main' with a function in `-lnosys':
AC_CHECK_LIB([nosys],[main])dnl
dnl# FIXME: Replace `main' with a function in `-lu8gbase':
AC_CHECK_LIB([u8gbase],[main])dnl
dnl# FIXME: Replace `main' with a function in `-lu8gsdl':
AC_CHECK_LIB([u8gsdl],[main])dnl
dnl# FIXME: Replace `main' with a function in `-lu8gwpi':
AC_CHECK_LIB([u8gwpi],[main])dnl
AC_SEARCH_LIBS([u8g_GetGlyph],[u8gbase u8gsdl u8gwpi u8glib u8g])dnl
AC_CHECK_LIB([wiringPi],[wiringPiSetup])dnl

# Checks for header files.
AC_HEADER_STDBOOL dnl# also "_CHECK"s it
AC_CHECK_HEADERS([fcntl.h float.h stddef.h sys/ioctl.h termios.h wiringPi.h \
                  wiringPiI2C.h wiringPiSPI.h])dnl

if test "x${ac_cv_header_inttypes_h}" = "x"; then
  test -z "${ac_cv_header_inttypes_h}" || unset ac_cv_header_inttypes_h
  AC_CHECK_HEADERS([inttypes.h])
fi
if test "x${ac_cv_header_stdint_h}" = "x"; then
  test -z "${ac_cv_header_stdint_h}" || unset ac_cv_header_stdint_h
  AC_CHECK_HEADERS([stdint.h])
fi
if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}" || unset ac_cv_header_unistd_h
  AC_CHECK_HEADERS([unistd.h])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_SYS_LARGEFILE
AC_C_CONST
AC_C_INLINE
AC_C_VOLATILE
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit digitalRead digitalWrite memset pinMode rand sqrt \
                strerror st_Setup u8g_GetGlyph usleep wiringPiSetup \
                wiringPiI2CSetup wiringPiI2CWriteReg8 wiringPiSPISetup \
                wiringPiSPIDataRW])dnl

# Output.
AC_CONFIG_FILES([Makefile])
AC_CONFIG_SUBDIRS([tools/font/otf2bdf])
AC_OUTPUT
