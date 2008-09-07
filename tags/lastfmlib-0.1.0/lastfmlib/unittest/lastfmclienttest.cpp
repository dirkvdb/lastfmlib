#include <unittest++/UnitTest++.h>

#include "lastfmlib/lastfmclient.h"
#include "lastfmlib/lastfmscrobbler.h"
#include <ctime>
#include <unistd.h>

using std::string;

SUITE(LastFmClientTest)
{
    TEST(TestLastFmClient)
    {
        //~ LastFmScrobbler scrobbler("liblastfmtest", "testlib");
        //~ SubmissionInfo info(L"Richie Hawtin", L"The Tunnel");
        //~ info.setTrackLength(35);
//~
        //~ scrobbler.startedPlaying(info);
        //~ sleep(20);
        //~ scrobbler.finishedPlaying();


        //~ LastFmClient lastFm("liblastfmtest", "testlib");
        //~ SubmissionInfo info(L"Tomaz, Cook-E & Matik, Alter Ego, Ed & Kim", L"Live @ Petrol (03-11-2007)", time(0) - 100);
        //~ info.setTrackLength(12457);
        //~ info.setRating(Love);
        //~ //lastFm.nowPlaying(info);
        //~ lastFm.submit(info);
    }
}
