#include "lastfmscrobbler.h"

#include "utils/scopedlock.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <signal.h>
#include <algorithm>

using namespace std;
using namespace utils;

static const time_t MIN_SECONDS_TO_SUBMIT       = 240;
static const time_t MIN_TRACK_LENGTH_TO_SUBMIT  = 30;
static const time_t MIN_SECS_BETWEEN_CONNECT    = 60;
static const time_t MAX_SECS_BETWEEN_CONNECT    = 7200;

LastFmScrobbler::LastFmScrobbler(const string& user, const string& pass, bool synchronous)
: m_pLastFmClient(new LastFmClient())
, m_LastConnectionAttempt(0)
, m_TrackPlayTime(-1)
, m_TrackResumeTime(0)
, m_AuthenticateThread(LastFmScrobbler::authenticateThread, this)
, m_SendInfoThread(LastFmScrobbler::sendInfoThread, this)
, m_Authenticated(false)
, m_HardConnectionFailureCount(0)
, m_Username(user)
, m_Password(pass)
, m_Log("/tmp/lastfmliblog.txt")
, m_Synchronous(synchronous)
{
    authenticateIfNecessary();
}

LastFmScrobbler::LastFmScrobbler(bool synchronous)
: m_pLastFmClient(NULL)
, m_LastConnectionAttempt(0)
, m_TrackPlayTime(-1)
, m_TrackResumeTime(0)
, m_AuthenticateThread(LastFmScrobbler::authenticateThread, this)
, m_SendInfoThread(LastFmScrobbler::sendInfoThread, this)
, m_Authenticated(false)
, m_HardConnectionFailureCount(0)
, m_Log("/tmp/lastfmliblog.txt")
, m_Synchronous(synchronous)
{
}

LastFmScrobbler::~LastFmScrobbler()
{
    joinThreads();
    delete m_pLastFmClient;
}

void LastFmScrobbler::startedPlaying(const SubmissionInfo& info)
{
    authenticateIfNecessary();

    m_Log.info("startedPlaying " + info.getTrack());
    m_PreviousTrackInfo = m_CurrentTrackInfo;
    m_CurrentTrackInfo = info;

    m_TrackPlayTime = 0;
    m_TrackResumeTime = m_CurrentTrackInfo.getTimeStarted();

    if (m_CurrentTrackInfo.getTimeStarted() < 0)
    {
        m_CurrentTrackInfo.setTimeStarted(time(NULL));
    }

    if (m_Synchronous)
    {
        submitTrack(m_PreviousTrackInfo);
        setNowPlaying();
    }
    else
    {
        m_SendInfoThread.start();
    }
}

void LastFmScrobbler::pausePlaying(bool paused)
{
    time_t curTime = time(NULL);
    if (paused)
    {
        m_TrackPlayTime += curTime - m_CurrentTrackInfo.getTimeStarted();
    }
    else
    {
        m_TrackResumeTime = curTime;
    }
}

void LastFmScrobbler::finishedPlaying()
{
    authenticateIfNecessary();
    if (m_Synchronous)
    {
        submitTrack(m_CurrentTrackInfo);
    }
    else
    {
        m_Log.error("async finish not implemented yet!!");
    }
}

bool LastFmScrobbler::trackCanBeCommited(const SubmissionInfo& info)
{
    time_t curTime = time(NULL);
    printf("playtime: %d - %d\n", m_TrackPlayTime, m_TrackResumeTime);
    m_TrackPlayTime += curTime - m_TrackResumeTime;

    bool trackTooShort = info.getTrackLength() < MIN_TRACK_LENGTH_TO_SUBMIT;
    bool trackPlayedLongEnough =
            m_TrackPlayTime >= MIN_SECONDS_TO_SUBMIT
        ||  m_TrackPlayTime >= (info.getTrackLength() / 2);

    printf("%d\n", curTime);
    printf("%d - %d - %d\n", m_TrackPlayTime, info.getTrackLength() / 2, m_TrackResumeTime);

    if (trackTooShort)
    {
        m_Log.info("Track \"" + info.getTrack() + "\" can't be committed: length is too short");
    }
    else if (!trackPlayedLongEnough)
    {
        stringstream ss;
        ss << "playtime: " << m_TrackPlayTime << " - " << m_TrackResumeTime;
        m_Log.info(ss.str());
        m_Log.info("Track \"" + info.getTrack() + "\" can't be committed: not played long enough");
    }
    else
    {
        m_Log.info("Track \"" + info.getTrack() + "\" can be committed: conditions OK");
    }

    return (!trackTooShort) && trackPlayedLongEnough;
}

void LastFmScrobbler::authenticateIfNecessary()
{
    if (!m_Authenticated &&
        !m_AuthenticateThread.isRunning() &&
        canReconnect())
    {
        if (m_Synchronous)
        {
            authenticate();
        }
        else
        {
            m_AuthenticateThread.start();
        }
    }
}

void LastFmScrobbler::authenticate()
{
    try
    {
        m_pLastFmClient->handshake(m_Username, m_Password);
        m_Log.info("Authentication successfull for user: " + m_Username);
        m_HardConnectionFailureCount = 0;
        m_Authenticated = true;
    }
    catch (ConnectionError& e)
    {
        ++m_HardConnectionFailureCount;
        m_LastConnectionAttempt = time(NULL);
    }
    catch (logic_error& e)
    {
        m_Log.error(e.what());
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
    pScrobbler->m_Log.info("Authenticate thread started");

    pScrobbler->authenticate();

    {
        ScopedLock lock(pScrobbler->m_AuthenticatedMutex);
        pScrobbler->m_AuthenticatedCondition.broadcast();
    }

    pScrobbler->m_Log.info("Authenticate thread finished");
    return NULL;
}

void* LastFmScrobbler::sendInfoThread(void* pInstance)
{
    LastFmScrobbler* pScrobbler = reinterpret_cast<LastFmScrobbler*>(pInstance);
    pScrobbler->m_Log.info("sendInfo thread started");

    {
        ScopedLock lock(pScrobbler->m_AuthenticatedMutex);
        if (!pScrobbler->m_Authenticated)
        {
            if (!pScrobbler->m_AuthenticatedCondition.wait(pScrobbler->m_AuthenticatedMutex, 4000))
            {
                pScrobbler->m_Log.info("send info terminated because no connection");
                pScrobbler->submitTrack(pScrobbler->m_PreviousTrackInfo);
                return NULL;
            }
        }
    }

    if (pScrobbler->m_Authenticated)
    {
        pScrobbler->submitTrack(pScrobbler->m_PreviousTrackInfo);
        pScrobbler->setNowPlaying();
    }

    pScrobbler->m_Log.info("sendInfo thread finished");
    return NULL;
}

void LastFmScrobbler::setNowPlaying()
{
    if (!m_Authenticated)
    {
        m_Log.info("Can't set Now Playing status: not authenticated");
        return;
    }

    try
    {
        m_pLastFmClient->nowPlaying(m_CurrentTrackInfo);
        m_Log.info("Now playing info submitted: " + m_CurrentTrackInfo.getArtist() + " - " + m_CurrentTrackInfo.getTrack());
    }
    catch (ConnectionError& e)
    {
        m_Authenticated = false;
    }
    catch (logic_error& e)
    {
        m_Log.error(e.what());
    }
}

void LastFmScrobbler::submitTrack(const SubmissionInfo& info)
{
    if (info.getTrackLength() < 0 || !trackCanBeCommited(info))
    {
        m_Log.info("Won't submit");
        return;
    }
    else
    {
        ScopedLock lock(m_TrackInfosMutex);
        m_BufferedTrackInfos.addInfo(info);
    }

    SubmissionInfoCollection tracksToSubmit;
    {
        ScopedLock lock(m_TrackInfosMutex);
        tracksToSubmit = m_BufferedTrackInfos;
    }

    try
    {
        if (m_Authenticated)
        {
            m_pLastFmClient->submit(tracksToSubmit);
            m_Log.info("Buffered tracks submitted");
            m_BufferedTrackInfos.clear();
        }
        else
        {
            m_Log.info("Track info buffered: not connected");
        }
    }
    catch (ConnectionError& e)
    {
        m_Authenticated = false;
    }
    catch (logic_error& e)
    {
        m_Log.error(e.what());
    }
}

void LastFmScrobbler::joinThreads()
{
    if (m_Synchronous)
    {
        return;
    }

    {
        ScopedLock lock(m_AuthenticatedMutex);
        m_AuthenticatedCondition.broadcast();
    }

    m_AuthenticateThread.join();
    m_SendInfoThread.join();
}
