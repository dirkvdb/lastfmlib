#ifndef LAST_FM_SCROBBLER_H
#define LAST_FM_SCROBBLER_H

#include <string>
#include <ctime>

#include "lastfmclient.h"
#include "submissioninfo.h"
#include "submissioninfocollection.h"
#include "utils/condition.h"
#include "utils/mutex.h"
#include "utils/thread.h"


class LastFmScrobbler
{
public:
    LastFmScrobbler(const std::string& user, const std::string& pass);
    virtual ~LastFmScrobbler();

    void startedPlaying(const SubmissionInfo& info);
    void pausePlaying(bool paused);
    void finishedPlaying();

protected:
    LastFmClient*   m_pLastFmClient;
    time_t          m_LastConnectionAttempt;
    time_t          m_TrackPlayTime;
    time_t          m_TrackResumeTime;

private:
    void authenticateIfNecessary();
    bool trackCanBeCommited();
    bool canReconnect();

    void startThread(pthread_t* pThread, ThreadFunction pfnThreadFunction);
    void joinThreads();

    static void* authenticateThread(void* pInstance);
    static void* nowPlayingThread(void* pInstance);
    static void* submitTrackThread(void* pInstance);

    Thread          m_AuthenticateThread;
    Thread          m_NowPlayingThread;
    Thread          m_SubmitTrackThread;

    SubmissionInfo  m_CurrentTrackInfo;
    SubmissionInfoCollection m_BufferedTrackInfos;

    bool            m_Authenticated;
    int             m_HardConnectionFailureCount;
    Condition       m_AuthenticatedCondition;
    Mutex           m_AuthenticatedMutex;
    Mutex           m_TrackInfosMutex;

    std::string     m_Username;
    std::string     m_Password;
};

#endif
