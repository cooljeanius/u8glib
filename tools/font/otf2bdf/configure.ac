dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([otf2bdf],[3.1],[mleisher@crl.nmsu.edu])
AC_CONFIG_SRCDIR([otf2bdf.c])
AC_CONFIG_HEADERS([config.h])
AC_REQUIRE_AUX_FILE([mkinstalldirs])dnl

dnl# since we don't use automake in this subdir, we have to open-code this instead:
AC_MSG_CHECKING([whether to enable maintainer-specific portions of Makefiles])
AC_ARG_ENABLE([maintainer-mode],
[AS_HELP_STRING([--enable-maintainer-mode],[enable make rules and
                          dependencies not useful (and sometimes confusing)
                          to the casual installer])],
      [USE_MAINTAINER_MODE=${enableval}],
      [USE_MAINTAINER_MODE=no])
AC_MSG_RESULT([${USE_MAINTAINER_MODE}])
AC_SUBST([MAINTAINER_MODE_TRUE])dnl
AC_SUBST([MAINTAINER_MODE_FALSE])dnl
  ## Pass the result down to the Makefile:
if test "x${USE_MAINTAINER_MODE}" = "xyes"; then
  MAINTAINER_MODE_TRUE=""
  MAINTAINER_MODE_FALSE='#'
else
  MAINTAINER_MODE_TRUE='#'
  MAINTAINER_MODE_FALSE=""
fi
MAINT="${MAINTAINER_MODE_TRUE}"
AC_SUBST([MAINT])dnl

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

AS_IF([test "x${CC}" = "xgcc" && test "x${GCC}" = "xyes"],[
  XX_CFLAGS="-ansi -Wall -Wextra -Werror=shadow -Wwrite-strings -pedantic -Wno-long-long"
  AS_IF([test "x${ac_ct_CC}" = "xgcc"],[
    XX_CFLAGS="${XX_CFLAGS} -Werror=declaration-after-statement -Wconversion"
  ])dnl
],[
  AS_CASE(["${host}"],
    [alpha-dec-osf*],[XX_CFLAGS="-std1 -O2 -g3"],
    [*-*-*],[XX_CFLAGS=""])dnl
])dnl
AC_SUBST([XX_CFLAGS])dnl

AC_CHECK_PROG([RM],[rm],[rm]) ##
AC_CHECK_PROG([CP],[cp],[cp])dnl

dnl# Checks for header files.
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
