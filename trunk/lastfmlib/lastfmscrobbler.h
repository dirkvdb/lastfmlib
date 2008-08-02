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
#include "utils/log.h"


class LastFmScrobbler
{
public:
    LastFmScrobbler(const std::string& user, const std::string& pass);
    virtual ~LastFmScrobbler();

    void startedPlaying(const SubmissionInfo& info);
    void pausePlaying(bool paused);
    void finishedPlaying();

protected:
    LastFmScrobbler();
    LastFmClient*   m_pLastFmClient;
    time_t          m_LastConnectionAttempt;
    time_t          m_TrackPlayTime;
    time_t          m_TrackResumeTime;
    utils::Thread   m_AuthenticateThread;
    utils::Thread   m_SendInfoThread;

private:
    void authenticateIfNecessary();
    bool trackCanBeCommited(const SubmissionInfo& info);
    bool canReconnect();
    void submitTrack(const SubmissionInfo& info);
    void setNowPlaying();

    void startThread(pthread_t* pThread, utils::ThreadFunction pfnThreadFunction);
    void joinThreads();

    static void* authenticateThread(void* pInstance);
    static void* sendInfoThread(void* pInstance);

    SubmissionInfo              m_PreviousTrackInfo;
    SubmissionInfo              m_CurrentTrackInfo;
    SubmissionInfoCollection    m_BufferedTrackInfos;

    bool                        m_Authenticated;
    int                         m_HardConnectionFailureCount;
    utils::Condition            m_AuthenticatedCondition;
    utils::Mutex                m_AuthenticatedMutex;
    utils::Mutex                m_TrackInfosMutex;

    std::string                 m_Username;
    std::string                 m_Password;

    utils::Log                  m_Log;
};

#endif
