#include "lastfmscrobbler.h"

#include "utils/scopedlock.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <signal.h>
#include <algorithm>

using namespace std;

static const time_t MIN_SECONDS_TO_SUBMIT       = 240;
static const time_t MIN_TRACK_LENGTH_TO_SUBMIT  = 30;
static const time_t MIN_SECS_BETWEEN_CONNECT    = 60;
static const time_t MAX_SECS_BETWEEN_CONNECT    = 7200;

LastFmScrobbler::LastFmScrobbler(const string& user, const string& pass)
: m_pLastFmClient(new LastFmClient())
, m_LastConnectionAttempt(0)
, m_TrackPlayTime(0)
, m_TrackResumeTime(0)
, m_AuthenticateThread(LastFmScrobbler::authenticateThread, this)
, m_NowPlayingThread(LastFmScrobbler::nowPlayingThread, this)
, m_SubmitTrackThread(LastFmScrobbler::submitTrackThread, this)
, m_Authenticated(false)
, m_HardConnectionFailureCount(0)
, m_Username(user)
, m_Password(pass)
{
    //authenticateIfNecessary();
}

LastFmScrobbler::~LastFmScrobbler()
{
    joinThreads();
    delete m_pLastFmClient;
}

void LastFmScrobbler::startedPlaying(const SubmissionInfo& info)
{
    authenticateIfNecessary();

    m_CurrentTrackInfo = info;

    if (m_CurrentTrackInfo.getTimeStarted() < 0)
    {
        m_CurrentTrackInfo.setTimeStarted(time(NULL));
    }

    m_TrackPlayTime = 0;
    m_TrackResumeTime = m_CurrentTrackInfo.getTimeStarted();

    cout << "Start nowplaying thread" << endl;
    m_NowPlayingThread.start();
}

void LastFmScrobbler::pausePlaying(bool paused)
{
    time_t curTime = time(NULL);
    if (paused)
    {
        m_TrackPlayTime += curTime - m_TrackResumeTime;
    }
    else
    {
        m_TrackResumeTime = curTime;
    }
}

void LastFmScrobbler::finishedPlaying()
{
    authenticateIfNecessary();

    if (trackCanBeCommited())
    {
        {
            ScopedLock lock(m_TrackInfosMutex);
            m_BufferedTrackInfos.addInfo(m_CurrentTrackInfo);
        }

        if (m_Authenticated)
        {
            cout << "start commit thread" << endl;
            m_SubmitTrackThread.start();
        }
        else
        {
            cout << "trackinfo buffered" << endl;
        }
    }

    m_TrackPlayTime = 0;
}

bool LastFmScrobbler::trackCanBeCommited()
{
    time_t curTime = time(NULL);
    m_TrackPlayTime += curTime - m_TrackResumeTime;

    if (m_CurrentTrackInfo.getTrackLength() >= MIN_TRACK_LENGTH_TO_SUBMIT ||
        m_TrackPlayTime >= MIN_SECONDS_TO_SUBMIT ||
        m_TrackPlayTime >= (m_CurrentTrackInfo.getTrackLength() / 2))
    {
        return true;
    }

    return false;
}

void LastFmScrobbler::authenticateIfNecessary()
{
    if (!m_Authenticated &&
        !m_AuthenticateThread.isRunning() &&
        canReconnect())
    {
        cout << "start authenticatethread" << endl;
        m_AuthenticateThread.start();
    }
}

bool LastFmScrobbler::canReconnect()
{
    time_t curTime = time(NULL);
    time_t timeSinceLastConnectionAttempt = curTime - m_LastConnectionAttempt;
    time_t connectionDelay = max(MAX_SECS_BETWEEN_CONNECT, m_HardConnectionFailureCount * MIN_SECS_BETWEEN_CONNECT);

    return timeSinceLastConnectionAttempt > connectionDelay;
}

void* LastFmScrobbler::authenticateThread(void* pInstance)
{
    LastFmScrobbler* pScrobbler = reinterpret_cast<LastFmScrobbler*>(pInstance);

    try
    {
        pScrobbler->m_pLastFmClient->handshake(pScrobbler->m_Username, pScrobbler->m_Password);
        pScrobbler->m_HardConnectionFailureCount = 0;
        pScrobbler->m_Authenticated = true;

        ScopedLock lock(pScrobbler->m_AuthenticatedMutex);
        pScrobbler->m_AuthenticatedCondition.broadcast();
    }
    catch (ConnectionError& e)
    {
        ++(pScrobbler->m_HardConnectionFailureCount);
        pScrobbler->m_LastConnectionAttempt = time(NULL);
    }
    catch (logic_error& e)
    {
        cerr << e.what() << endl;
    }

    return NULL;
}

void* LastFmScrobbler::nowPlayingThread(void* pInstance)
{
    LastFmScrobbler* pScrobbler = reinterpret_cast<LastFmScrobbler*>(pInstance);

    {
        ScopedLock lock(pScrobbler->m_AuthenticatedMutex);
        if (!pScrobbler->m_Authenticated)
        {
            if (!pScrobbler->m_AuthenticatedCondition.wait(pScrobbler->m_AuthenticatedMutex, 2000))
            {
                cout << "now playing terminated because no connection" << endl;
                return NULL;
            }
        }
    }

    try
    {
        pScrobbler->m_pLastFmClient->nowPlaying(pScrobbler->m_CurrentTrackInfo);
    }
    catch (ConnectionError& e)
    {
        pScrobbler->m_Authenticated = false;
    }
    catch (logic_error& e)
    {
        cerr << e.what() << endl;
    }

    return NULL;
}

void* LastFmScrobbler::submitTrackThread(void* pInstance)
{
    LastFmScrobbler* pScrobbler = reinterpret_cast<LastFmScrobbler*>(pInstance);

    try
    {
        SubmissionInfoCollection tracksToSubmit;
        {
            ScopedLock lock(pScrobbler->m_TrackInfosMutex);
            tracksToSubmit = pScrobbler->m_BufferedTrackInfos;
            pScrobbler->m_BufferedTrackInfos.clear();
        }

        pScrobbler->m_pLastFmClient->submit(tracksToSubmit);
    }
    catch (ConnectionError& e)
    {
        pScrobbler->m_Authenticated = false;
    }
    catch (logic_error& e)
    {
        cerr << e.what() << endl;
    }

    return NULL;
}

void LastFmScrobbler::joinThreads()
{
    {
        ScopedLock lock(m_AuthenticatedMutex);
        m_AuthenticatedCondition.broadcast();
    }

    m_NowPlayingThread.join();
    m_SubmitTrackThread.join();
    m_AuthenticateThread.join();
}
