SUBDIRS = lastfmlib

lib_LTLIBRARIES = liblastfm.la

liblastfm_la_SOURCES        =   lastfmlib/urlclient.h \
				lastfmlib/urlclient.cpp \
                                lastfmlib/lastfmclient.h \
				lastfmlib/lastfmclient.cpp \
                                lastfmlib/lastfmscrobbler.h \
				lastfmlib/lastfmscrobbler.cpp \
                                lastfmlib/nowplayinginfo.h \
				lastfmlib/nowplayinginfo.cpp \
                                lastfmlib/submissioninfo.h \
				lastfmlib/submissioninfo.cpp \
                                lastfmlib/submissioninfocollection.h \
				lastfmlib/submissioninfocollection.cpp \
                                lastfmlib/lastfmexceptions.h \
                                lastfmlib/md5/md5.h \
				lastfmlib/md5/md5.c \
                                lastfmlib/utils/stringoperations.h \
				lastfmlib/utils/stringoperations.cpp \
                                lastfmlib/utils/mutex.h \
				lastfmlib/utils/mutex.cpp \
                                lastfmlib/utils/thread.h \
				lastfmlib/utils/thread.cpp \
                                lastfmlib/lastfmlib/utils/scopedlock.h \
				lastfmlib/utils/scopedlock.cpp \
                                lastfmlib/utils/condition.h \
				lastfmlib/utils/condition.cpp \
                                lastfmlib/utils/log.h \
				lastfmlib/utils/log.cpp

nobase_include_HEADERS   =   	lastfmlib/lastfmclient.h \
                                lastfmlib/lastfmscrobbler.h \
                                lastfmlib/nowplayinginfo.h \
                                lastfmlib/submissioninfo.h \
                                lastfmlib/submissioninfocollection.h \
                                lastfmlib/lastfmexceptions.h \
                                lastfmlib/urlclient.h \
                                lastfmlib/utils/thread.h \
                                lastfmlib/utils/mutex.h \
                                lastfmlib/utils/scopedlock.h \
                                lastfmlib/utils/condition.h \
                                lastfmlib/utils/log.h

#liblastfm_includedir        =   $(includedir)/lastfmlib
liblastfm_la_CXXFLAGS       =   $(LOG4CPP_CFLAGS)
liblastfm_la_LDFLAGS        =   -version-info 1:0:0
liblastfm_la_LIBADD         =   $(CURL_LIBS) $(LOG4CPP_LIBS)

AM_CXXFLAGS = -Wall -Wfatal-errors -Werror
ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = @pkgconfigdir@
pkgconfig_DATA = lastfmlib.pc
