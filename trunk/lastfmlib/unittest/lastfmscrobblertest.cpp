#include <unittest++/UnitTest++.h>

#include "lastfmclientmock.h"
#include "lastfmlib/lastfmscrobbler.h"

#include <ctime>
#include <unistd.h>
#include <iostream>

using namespace std;

class LastFmScrobblerTester : public LastFmScrobbler
{
public:
    LastFmScrobblerTester(const std::string& user, const std::string& pass)
    : LastFmScrobbler(user, pass)
    {
        delete m_pLastFmClient;
        pMock = new LastFmClientMock();
        m_pLastFmClient = pMock;
    }

    void setLastConnectionAttempt(time_t time)
    {
        m_LastConnectionAttempt = time;
    }

    void setTrackPlayTime(time_t time)
    {
        m_TrackPlayTime = time;
    }

    void setTrackResumeTime(time_t time)
    {
        m_TrackResumeTime = time;
    }

    LastFmClientMock* pMock;
};

SUITE(LastFmScrobblerTest)
{
    TEST(TestLastFmScrobbler)
    {
        return;
        LastFmScrobblerTester scrobbler("liblastfmtest", "testlib");

        SubmissionInfo info("Artist", "Track");

        scrobbler.startedPlaying(info);
        usleep(100000);
        CHECK(scrobbler.pMock->m_NowPlayingCalled);
        CHECK_EQUAL("Artist", scrobbler.pMock->m_LastRecPlayingInfo.getArtist());
    }

    TEST(TestLastFmScrobblerConnectionFailed)
    {
        LastFmScrobblerTester scrobbler("liblastfmtest", "testlib");
        scrobbler.pMock->m_HandShakeThrowConnectionError = true;

        SubmissionInfo info1("Artist1", "Track1");
        info1.setTrackLength(100);

        scrobbler.startedPlaying(info1);
        usleep(100000);
        CHECK(!scrobbler.pMock->m_NowPlayingCalled);

        //make sure track has played long enough
        scrobbler.setTrackPlayTime(100);
        usleep(3000000);
        scrobbler.finishedPlaying();
        usleep(100000);
        CHECK(!scrobbler.pMock->m_NowPlayingCalled);
        CHECK(!scrobbler.pMock->m_SubmitCalled);
        CHECK(!scrobbler.pMock->m_SubmitCollectionCalled);

        //force immediate reconnect
        scrobbler.setLastConnectionAttempt(0);

        SubmissionInfo info2("Artist2", "Track2");
        info2.setTrackLength(100);

        scrobbler.pMock->m_HandShakeThrowConnectionError = false;
        scrobbler.startedPlaying(info2);
        usleep(100000);
        CHECK(scrobbler.pMock->m_NowPlayingCalled);
        CHECK(!scrobbler.pMock->m_SubmitCalled);
        CHECK(!scrobbler.pMock->m_SubmitCollectionCalled);

        //make sure track has played long enough
        scrobbler.setTrackPlayTime(100);
        scrobbler.finishedPlaying();
        usleep(100000);
        CHECK(!scrobbler.pMock->m_SubmitCalled);
        CHECK(scrobbler.pMock->m_SubmitCollectionCalled);

        string post = scrobbler.pMock->m_LastRecSubmitInfoCollection.getPostData();
        CHECK(string::npos != post.find("Artist1"));
        CHECK(string::npos != post.find("Artist2"));
    }
}
