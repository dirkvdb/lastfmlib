if ENABLE_UNITTEST
check_PROGRAMS = testrunner
endif


testrunner_SOURCES =    testrunner.cpp\
                        lastfmclienttest.cpp\
                        lastfmscrobblertest.cpp\
                        stringoperationstest.cpp\
                        nowplayinginfotest.cpp\
                        submissioninfotest.cpp\
                        submissioninfocollectiontest.cpp\
                        lastfmclientmock.h lastfmclientmock.cpp

testrunner_LDFLAGS  =   $(top_builddir)/lastfmlib/liblastfm.la
testrunner_LDADD    =   $(UNITTEST_LIBS) $(CURL_LIBS) $(LOG4CPP_LIBS)
INCLUDES            =   -I$(top_srcdir)

AM_CXXFLAGS = -Wall -Wfatal-errors

