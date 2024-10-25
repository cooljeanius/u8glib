dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([otf2bdf],[3.1],[mleisher@crl.nmsu.edu])
AC_CONFIG_SRCDIR([otf2bdf.c])
AC_CONFIG_HEADERS([config.h])
AC_REQUIRE_AUX_FILE([mkinstalldirs])dnl

dnl# Checks for programs.
AC_PROG_CC
AC_PATH_PROG([FREETYPE_CONFIG],[freetype-config],[false])dnl

OLDLIBS="${LIBS}"
if test "x${FREETYPE_CONFIG}" != "x" && test "x${FREETYPE_CONFIG}" != "xfalse" && test -x "${FREETYPE_CONFIG}"; then
  LIBS="${LIBS} `freetype-config --libs`"
  CPPFLAGS="${CPPFLAGS} `freetype-config --cflags`"
elif test -x "`which pkg-config`" && pkg-config --exists freetype2; then
  LIBS="${LIBS} `pkg-config --libs freetype2`"
  CPPFLAGS="${CPPFLAGS} `pkg-config --cflags freetype2`"
else
  LIBS="${LIBS} -lfreetype"
  CPPFLAGS="${CPPFLAGS} -Werror"
fi
AC_CHECK_LIB([freetype],[FT_Init_FreeType],[LIBS="${LIBS} -lfreetype"],
    [AC_MSG_ERROR([Cannot find Freetype library! Compile FreeType first.])])
AC_SUBST([OLDLIBS])dnl
AC_SUBST([LIBS])dnl

dnl# get Compiler flags right.

if test "x${CC}" = "xgcc"; then
	XX_CFLAGS="-Wall -pedantic"
else
	case "${host}" in
  		alpha-dec-osf*)
        	XX_CFLAGS="-std1 -O2 -g3"
       		;;
  		*)
          	XX_CFLAGS=""
       		;;
	esac
fi
AC_SUBST([XX_CFLAGS])dnl

AC_CHECK_PROG([RM],[rm],[rm]) ##
AC_CHECK_PROG([CP],[cp],[cp])dnl

dnl# Checks for header files.
AC_DEFUN([AC_REQUIRE_HEADER_STDC],[
  AC_REQUIRE([AC_HEADER_STDC])dnl
])dnl
AC_REQUIRE_HEADER_STDC
AC_CHECK_HEADERS([windows.h])dnl

if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}" || unset ac_cv_header_unistd_h
  AC_CHECK_HEADERS([unistd.h])
fi

dnl# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_MEMCMP
AC_CHECK_FUNCS([memset strrchr])dnl

dnl# Output.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
