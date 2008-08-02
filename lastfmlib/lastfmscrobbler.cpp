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

LastFmScrobbler::LastFmScrobbler(const string& user, const string& pass)
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
{
    authenticateIfNecessary();
}

LastFmScrobbler::LastFmScrobbler()
: m_pLastFmClient(NULL)
, m_LastConnectionAttempt(0)
, m_TrackPlayTime(-1)
, m_TrackResumeTime(0)
, m_AuthenticateThread(LastFmScrobbler::authenticateThread, this)
, m_SendInfoThread(LastFmScrobbler::sendInfoThread, this)
, m_Authenticated(false)
, m_HardConnectionFailureCount(0)
, m_Log("/tmp/lastfmliblog.txt")
{
    m_Log.info("__________________________________");
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

    if (m_CurrentTrackInfo.getTimeStarted() < 0)
    {
        m_CurrentTrackInfo.setTimeStarted(time(NULL));
    }

    m_Log.info("Start sendInfo thread");
    m_SendInfoThread.start();
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
    submitTrack(m_CurrentTrackInfo);
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
        m_Log.info("start authenticatethread");
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
        pScrobbler->m_Log.info("Authentication successfull for user: " + pScrobbler->m_Username);
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
        pScrobbler->m_Log.error(e.what());
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
            if (!pScrobbler->m_AuthenticatedCondition.wait(pScrobbler->m_AuthenticatedMutex, 2000))
            {
                pScrobbler->m_Log.info("send info terminated because no connection");
                pScrobbler->submitTrack(pScrobbler->m_PreviousTrackInfo);
                pScrobbler->m_TrackPlayTime = 0;
                pScrobbler->m_TrackResumeTime = pScrobbler->m_CurrentTrackInfo.getTimeStarted();
                return NULL;
            }
            pScrobbler->m_Log.info("Good to go");
        }
    }

    pScrobbler->submitTrack(pScrobbler->m_PreviousTrackInfo);
    pScrobbler->m_TrackPlayTime = 0;
    pScrobbler->m_TrackResumeTime = pScrobbler->m_CurrentTrackInfo.getTimeStarted();
    pScrobbler->setNowPlaying();

    pScrobbler->m_Log.info("sendInfo thread finished");
    return NULL;
}

void LastFmScrobbler::setNowPlaying()
{
    printf("set m_TrackResumeTime: %d\n", m_TrackResumeTime);

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
    {
        ScopedLock lock(m_AuthenticatedMutex);
        m_AuthenticatedCondition.broadcast();
    }

    m_AuthenticateThread.join();
    m_SendInfoThread.join();
}
