SUBDIRS = md5 utils unittest

lib_LTLIBRARIES = liblastfm.la

liblastfm_la_SOURCES        =   urlclient.h urlclient.cpp\
                                lastfmclient.h lastfmclient.cpp\
                                lastfmscrobbler.h lastfmscrobbler.cpp\
                                nowplayinginfo.h nowplayinginfo.cpp\
                                submissioninfo.h submissioninfo.cpp\
                                submissioninfocollection.h submissioninfocollection.cpp\
                                lastfmexceptions.h

liblastfm_include_HEADERS   =   lastfmclient.h\
                                lastfmscrobbler.h\
                                nowplayinginfo.h\
                                submissioninfo.h\
                                submissioninfocollection.h\
                                lastfmexceptions.h

liblastfm_includedir        =   $(includedir)/lastfmlib
liblastfm_la_LDFLAGS        =   -version-info 1:0:0
liblastfm_la_LIBADD         =   $(CURL_LIBS)\
                                utils/libutils.la\
                                md5/libmd5.la

AM_CXXFLAGS = -Wall -Wfatal-errors -Werror
