#include <gtest/gtest.h>

#include "lastfmclientmock.h"
#include "lastfmlib/lastfmscrobbler.h"

#include <ctime>
#include <unistd.h>
#include <iostream>

using namespace std;

class LastFmScrobblerTester : public LastFmScrobbler
{
public:
    LastFmScrobblerTester(bool synchronous)
    : LastFmScrobbler(synchronous)
    {
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

    void waitForAuthenticationFinish()
    {
        m_AuthenticateThread.join();
    }

    void waitForSendInfoFinish()
    {
        m_SendInfoThread.join();
    }

    LastFmClientMock* pMock;
};


TEST(LastFmScrobblerTest, LastFmScrobbler)
{
    LastFmScrobblerTester scrobbler(true);

    SubmissionInfo info("Artist", "Track");

    scrobbler.startedPlaying(info);
    EXPECT_TRUE(scrobbler.pMock->m_NowPlayingCalled);
    EXPECT_TRUE(!scrobbler.pMock->m_SubmitCalled);
    EXPECT_TRUE(!scrobbler.pMock->m_SubmitCollectionCalled);
    EXPECT_EQ("Artist", scrobbler.pMock->m_LastRecPlayingInfo.getArtist());
}

TEST(LastFmScrobblerTest, LastFmScrobblerConnectionFailed)
{
    LastFmScrobblerTester scrobbler(true);
    scrobbler.pMock->m_HandShakeThrowConnectionError = true;

    SubmissionInfo info1("Artist1", "Track1");
    info1.setTrackLength(100);

    SubmissionInfo info2("Artist2", "Track2");
    info2.setTrackLength(100);

    SubmissionInfo info3("Artist3", "Track3");
    scrobbler.startedPlaying(info1);
    EXPECT_TRUE(!scrobbler.pMock->m_NowPlayingCalled);

    //make sure track has played long enough
    scrobbler.setTrackPlayTime(100);
    scrobbler.startedPlaying(info2);
    usleep(1000000);
    EXPECT_TRUE(!scrobbler.pMock->m_NowPlayingCalled);
    EXPECT_TRUE(!scrobbler.pMock->m_SubmitCalled);
    EXPECT_TRUE(!scrobbler.pMock->m_SubmitCollectionCalled);

    //force immediate reconnect
    scrobbler.pMock->m_HandShakeThrowConnectionError = false;
    scrobbler.setLastConnectionAttempt(0);
    scrobbler.setTrackPlayTime(100);
    scrobbler.startedPlaying(info3);
    EXPECT_TRUE(scrobbler.pMock->m_NowPlayingCalled);
    EXPECT_TRUE(!scrobbler.pMock->m_SubmitCalled);
    EXPECT_TRUE(scrobbler.pMock->m_SubmitCollectionCalled);

    string post = scrobbler.pMock->m_LastRecSubmitInfoCollection.getPostData();
    EXPECT_TRUE(string::npos != post.find("Artist1"));
    EXPECT_TRUE(string::npos != post.find("Artist2"));
}

TEST(LastFmScrobblerTest, LastFmScrobblerBadSession)
{
    LastFmScrobblerTester scrobbler(true);
    scrobbler.pMock->m_BadSessionError = true;

    SubmissionInfo info1("Artist1", "Track1");
    info1.setTrackLength(100);

    scrobbler.startedPlaying(info1);

    EXPECT_TRUE(scrobbler.pMock->m_NowPlayingCalled);
    EXPECT_TRUE(scrobbler.pMock->m_HandshakeCalled);

    scrobbler.pMock->m_NowPlayingCalled = false;
    scrobbler.pMock->m_HandshakeCalled = false;

    scrobbler.pMock->m_BadSessionError = true;
    scrobbler.setTrackPlayTime(100);
    scrobbler.setCommitOnlyMode(true);
    scrobbler.startedPlaying(info1);

    EXPECT_TRUE(!scrobbler.pMock->m_NowPlayingCalled);
    EXPECT_TRUE(scrobbler.pMock->m_SubmitCollectionCalled);
    EXPECT_TRUE(scrobbler.pMock->m_HandshakeCalled);
}

