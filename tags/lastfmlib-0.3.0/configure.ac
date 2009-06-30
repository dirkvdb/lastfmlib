AC_INIT([lastfmlib], [0.3.0])
AC_CONFIG_SRCDIR([lastfmlib/lastfmscrobbler.cpp])
AM_INIT_AUTOMAKE

AC_LANG_CPLUSPLUS
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_CONFIG_MACRO_DIR([m4])
AC_HEADER_STDC

PKG_CHECK_MODULES(CURL, libcurl, HAVE_CURL=yes, AC_MSG_ERROR([ +Could not find libcurl]))

AC_SUBST(CURL_LIBS)
AC_SUBST(CURL_CFLAGS)

AC_ARG_WITH(pkgconfigdir,
    AC_HELP_STRING([--with-pkgconfigdir],
    [Use the specified pkgconfig dir (default is libdir/pkgconfig)]),
    [pkgconfigdir=${withval}],
    [pkgconfigdir='${libdir}/pkgconfig'])

AC_ARG_ENABLE(debug,
   [  --enable-debug Build in debug mode ],
   DEBUG=$enableval)

AM_CONDITIONAL(DEBUG, test "$enable_debug" = "yes")

if test "$DEBUG" = "yes"; then
    CXXFLAGS="-g -O1 -Wall -Werror -Wfatal-errors"
fi

CFLAGS="$CFLAGS -D_REENTRANT"
CXXFLAGS="$CXXFLAGS -D_REENTRANT"

AC_ARG_ENABLE(logging,
   [  --enable-logging Enables logging of debug messages to file ],
   ENABLE_LOGGING=$enableval)

if test "$ENABLE_LOGGING" = "yes"; then
    AC_CHECK_HEADER(syslog.h)
    AC_DEFINE(ENABLE_LOGGING)
fi
AM_CONDITIONAL(ENABLE_LOGGING, test "$enable_logging" = "yes")


AC_ARG_ENABLE(unittests,
   [  --enable-unittests Enables build of unittests ],
   ENABLE_UNITTEST=$enableval)

if test "$ENABLE_UNITTEST" = "yes"; then
    AC_CHECK_HEADERS(
    [gtest/gtest.h],,
    [AC_MSG_ERROR([Missing gtest library, install the google unittest framework])])
    UNITTEST_LIBS="$(gtest-config --libs)"
    AC_SUBST(UNITTEST_LIBS)
fi
AM_CONDITIONAL(ENABLE_UNITTEST, test "$enable_unittests" = "yes")

AC_SUBST([pkgconfigdir])
AC_MSG_NOTICE([pkgconfig directory is ${pkgconfigdir}])

AC_CONFIG_FILES([lastfmlib.pc:lastfmlib.pc.in])

AC_OUTPUT([
    Makefile
 ])

echo
echo "CONFIGURATION SUMMARY ----"

if test "x$ENABLE_LOGGING" = xyes; then
    echo "logging             : enabled"
else
    echo "logging             : disabled"
fi

if test "x$ENABLE_UNITTEST" = xyes; then
    echo "unittests           : enabled"
else
    echo "unittests           : disabled"
fi

if test "x$DEBUG" = xyes; then
    echo "debug mode          : enabled"
else
    echo "debug mode          : disabled"
fi
