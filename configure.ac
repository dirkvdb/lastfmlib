AC_INIT([lastfmlib], [0.1.0])
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

AC_ARG_ENABLE(logging,
   [  --enable-logging Enables logging of debug messages to file ],
   ENABLE_LOGGING=$enableval)

if test "$ENABLE_LOGGING" = "yes"; then
    PKG_CHECK_MODULES(LOG4CPP, log4cpp, HAVE_LOG4CPP=yes, AC_MSG_ERROR([ +Could not find log4cpp]))
    AC_SUBST(LOG4CPP_LIBS)
    AC_SUBST(LOG4CPP_CFLAGS)
    AC_DEFINE(ENABLE_LOGGING)
fi
AM_CONDITIONAL(ENABLE_LOGGING, test "$enable_logging" = "yes")


AC_ARG_ENABLE(unittests,
   [  --enable-unittests Enables build of unittests ],
   ENABLE_UNITTEST=$enableval)

if test "$ENABLE_UNITTEST" = "yes"; then
    PKG_CHECK_MODULES(UNITTEST, unittest++, HAVE_UNITTEST=yes, AC_MSG_ERROR([ +Could not find unittest++]))
    AC_SUBST(UNITTEST_LIBS)
    AC_SUBST(UNITTEST_CFLAGS)
    AC_DEFINE(ENABLE_UNITTEST)
fi
AM_CONDITIONAL(ENABLE_UNITTEST, test "$enable_unittests" = "yes")

AC_SUBST([pkgconfigdir])
AC_MSG_NOTICE([pkgconfig directory is ${pkgconfigdir}])

AC_CONFIG_FILES([lastfmlib.pc:lastfmlib.pc.in])

AC_OUTPUT([
    Makefile
    lastfmlib/Makefile
    lastfmlib/unittest/Makefile
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