SUBDIRS = unittest

lib_LTLIBRARIES = liblastfm.la

liblastfm_la_SOURCES        =   urlclient.h urlclient.cpp \
                                lastfmclient.h lastfmclient.cpp \
                                lastfmscrobbler.h lastfmscrobbler.cpp \
                                nowplayinginfo.h nowplayinginfo.cpp \
                                submissioninfo.h submissioninfo.cpp \
                                submissioninfocollection.h submissioninfocollection.cpp \
                                lastfmexceptions.h \
                                md5/md5.h md5/md5.c \
                                utils/stringoperations.h utils/stringoperations.cpp \
                                utils/mutex.h utils/mutex.cpp \
                                utils/thread.h utils/thread.cpp \
                                utils/scopedlock.h utils/scopedlock.cpp \
                                utils/condition.h utils/condition.cpp \
                                utils/log.h utils/log.cpp

liblastfm_include_HEADERS   =   lastfmclient.h \
                                lastfmscrobbler.h \
                                nowplayinginfo.h \
                                submissioninfo.h \
                                submissioninfocollection.h \
                                lastfmexceptions.h \
                                urlclient.h \
                                utils/thread.h \
                                utils/mutex.h \
                                utils/scopedlock.h \
                                utils/condition.h \
                                utils/log.h

liblastfm_includedir        =   $(includedir)/lastfmlib
liblastfm_la_CXXFLAGS       =   $(LOG4CPP_CFLAGS)
liblastfm_la_LDFLAGS        =   -version-info 1:0:0
liblastfm_la_LIBADD         =   $(CURL_LIBS) $(LOG4CPP_LIBS)

AM_CXXFLAGS = -Wall -Wfatal-errors -Werror
