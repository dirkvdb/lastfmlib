check_PROGRAMS = testrunner

testrunner_SOURCES =    testrunner.cpp\
                        lastfmclienttest.cpp\
                        lastfmscrobblertest.cpp\
                        stringoperationstest.cpp\
                        nowplayinginfotest.cpp\
                        submissioninfotest.cpp\
                        submissioninfocollectiontest.cpp\
                        lastfmclientmock.h lastfmclientmock.cpp

testrunner_LDFLAGS  =   $(top_builddir)/lastfmlib/liblastfm.la
testrunner_LDADD    =   -lunittest++ $(CURL_LIBS)
INCLUDES            =   -I$(top_srcdir)

AM_CXXFLAGS = -Wall -Wfatal-errors
