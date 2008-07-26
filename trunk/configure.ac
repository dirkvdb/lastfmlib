AC_INIT([liblastfm], [0.1.0])
AC_CONFIG_SRCDIR([lastfmlib/lastfmscrobbler.cpp])
AM_INIT_AUTOMAKE

AC_LANG_CPLUSPLUS
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_HEADER_STDC
AC_CHECK_HEADERS(
    [inttypes.h],,
    [AC_MSG_ERROR([Missing headers required to compile liblastfm])]
)

PKG_CHECK_MODULES(CURL, libcurl, HAVE_CURL=yes, AC_MSG_ERROR([ +Could not find libcurl])
)

AC_SUBST(CURL_LIBS)

AC_ARG_WITH(pkgconfigdir,
	AC_HELP_STRING([--with-pkgconfigdir],
	[Use the specified pkgconfig dir (default is libdir/pkgconfig)]),
	[pkgconfigdir=${withval}],
	[pkgconfigdir='${libdir}/pkgconfig'])

AC_SUBST([pkgconfigdir])
AC_MSG_NOTICE([pkgconfig directory is ${pkgconfigdir}])

AC_CONFIG_FILES([lastfmlib.pc:lastfmlib.pc.in])

AC_OUTPUT([
	Makefile
	lastfmlib/Makefile
	lastfmlib/md5/Makefile
	lastfmlib/utils/Makefile
	lastfmlib/unittest/Makefile
 ])
