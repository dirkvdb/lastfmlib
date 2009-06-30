lib_LTLIBRARIES = liblastfm.la

liblastfm_la_SOURCES        =   lastfmlib/urlclient.h \
                                lastfmlib/urlclient.cpp \
                                lastfmlib/lastfmclient.h \
                                lastfmlib/lastfmclient.cpp \
                                lastfmlib/lastfmscrobbler.h \
                                lastfmlib/lastfmscrobbler.cpp \
                                lastfmlib/lastfmscrobblerc.h \
                                lastfmlib/lastfmscrobblerc.cpp \
                                lastfmlib/nowplayinginfo.h \
                                lastfmlib/nowplayinginfo.cpp \
                                lastfmlib/submissioninfo.h \
                                lastfmlib/submissioninfo.cpp \
                                lastfmlib/submissioninfocollection.h \
                                lastfmlib/submissioninfocollection.cpp \
                                lastfmlib/lastfmexceptions.h \
                                lastfmlib/lastfmtypes.h \
                                lastfmlib/md5/md5.h \
                                lastfmlib/md5/md5.c \
                                lastfmlib/utils/stringoperations.h \
                                lastfmlib/utils/stringoperations.cpp \
                                lastfmlib/utils/mutex.h \
                                lastfmlib/utils/mutex.cpp \
                                lastfmlib/utils/thread.h \
                                lastfmlib/utils/thread.cpp \
                                lastfmlib/utils/scopedlock.h \
                                lastfmlib/utils/scopedlock.cpp \
                                lastfmlib/utils/condition.h \
                                lastfmlib/utils/condition.cpp \
                                lastfmlib/utils/log.h \
                                lastfmlib/utils/log.cpp

nobase_include_HEADERS   =      lastfmlib/lastfmclient.h \
                                lastfmlib/lastfmexceptions.h \
                                lastfmlib/lastfmtypes.h \
                                lastfmlib/lastfmscrobbler.h \
                                lastfmlib/lastfmscrobblerc.h \
                                lastfmlib/nowplayinginfo.h \
                                lastfmlib/submissioninfo.h \
                                lastfmlib/submissioninfocollection.h \
                                lastfmlib/urlclient.h \
                                lastfmlib/utils/thread.h \
                                lastfmlib/utils/mutex.h \
                                lastfmlib/utils/scopedlock.h \
                                lastfmlib/utils/condition.h \
                                lastfmlib/utils/log.h

liblastfm_la_CXXFLAGS       =   $(AM_CXXFLAGS)
liblastfm_la_LDFLAGS        =   -version-info 3:0:2
liblastfm_la_LIBADD         =   $(CURL_LIBS) -lpthread

ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = @pkgconfigdir@
pkgconfig_DATA = lastfmlib.pc

if ENABLE_UNITTEST
check_PROGRAMS = testrunner
endif

testrunner_SOURCES =    lastfmlib/unittest/testrunner.cpp \
                        lastfmlib/unittest/lastfmclienttest.cpp \
                        lastfmlib/unittest/lastfmscrobblertest.cpp \
                        lastfmlib/unittest/stringoperationstest.cpp \
                        lastfmlib/unittest/nowplayinginfotest.cpp \
                        lastfmlib/unittest/submissioninfotest.cpp \
                        lastfmlib/unittest/submissioninfocollectiontest.cpp \
                        lastfmlib/unittest/lastfmclientmock.h \
                        lastfmlib/unittest/lastfmclientmock.cpp

testrunner_CXXFLAGS	=   $(AM_CXXFLAGS)
testrunner_LDADD    =   liblastfm.la $(UNITTEST_LIBS) $(CURL_LIBS) -lpthread

