#include <gtest/gtest.h>

#include "lastfmlib/lastfmclient.h"
#include "lastfmlib/lastfmscrobbler.h"
#include "lastfmlib/lastfmscrobblerc.h"
#include <ctime>
#include <unistd.h>


using std::string;

TEST(LastFmClientTest, LastFmClient)
{
#if 0
    lastfm_scrobbler* scrobbler = create_scrobbler("liblastfmtest", "testlib", true);
    set_commit_only_mode(scrobbler, 1);

    submission_info* info = create_submission_info();
    info->artist = "Richie Hawtin";
    info->track = "The Tunnel";
    info->track_length_in_secs = 35;

    started_playing(scrobbler, info);
    destroy_submission_info(info);

    sleep(20);

    finished_playing(scrobbler);
    destroy_scrobbler(scrobbler);
#endif
#if 0
    LastFmScrobbler scrobbler("liblastfmtest", "testlib", true);
    SubmissionInfo info(L"Richie Hawtin", L"The Tunnel");
    info.setTrackLength(35);

    scrobbler.startedPlaying(info);
    sleep(20);
    scrobbler.finishedPlaying();
#endif

    //~ LastFmClient lastFm("liblastfmtest", "testlib", true);
    //~ SubmissionInfo info(L"Tomaz, Cook-E & Matik, Alter Ego, Ed & Kim", L"Live @ Petrol (03-11-2007)", time(0) - 100);
    //~ info.setTrackLength(12457);
    //~ info.setRating(Love);
    //~ //lastFm.nowPlaying(info);
    //~ lastFm.submit(info);
}
