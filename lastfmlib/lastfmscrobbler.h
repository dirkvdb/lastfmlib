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
    /** Constructor
     * \param user Last.fm user name
     * \param pass Last.fm password for user
     * \param synchronous if false all public methods will be executed in
     * a thread and return immediately (prevents long blocking methods in
     * case of network problems)
     */
    LastFmScrobbler(const std::string& user, const std::string& pass, bool synchronous);
    /** Destructor */
    virtual ~LastFmScrobbler();

    /** When commit only mode is set, now playinginfo is not updated on
     * Last.fm, tracks will only be commited once they finished playing
     * \param enabled set commitOnlyMode to true or false
     */
    void setCommitOnlyMode(bool enabled);

    /** Indicate that a new track has started playing, the previous track
     * will be submitted (if available) and the new track will be set as
     * Now Playing
     * \param info SubmissionInfo object containing information about
     * the new song
     */
    void startedPlaying(const SubmissionInfo& info);
    /** Indicate that the current track has stopped playing. The current
     * track will be submitted to Last.fm
     */
    void finishedPlaying();
    /** Indicate that playback of the current track has been (un)paused
     * \param paused true if track is being paused, false if being unpaused
     */
    void pausePlaying(bool paused);

protected:
    LastFmScrobbler(bool synchronous);
    LastFmClient*   m_pLastFmClient;
    /** \brief Last time a connection attempt was made */
    time_t          m_LastConnectionAttempt;
    /** \brief The time that the current track has been played, is set on pause */
    time_t          m_TrackPlayTime;
    /** \brief The time that the current track was resumed after a pause */
    time_t          m_TrackResumeTime;
    /** \brief Thread handle of authentication thread (protected for testing) */
    utils::Thread   m_AuthenticateThread;
    /** \brief Thread handle of sendinfo thread (protected for testing) */
    utils::Thread   m_SendInfoThread;


private:
    void authenticateIfNecessary();
    void authenticate();
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

    bool                        m_Synchronous;
    bool                        m_CommitOnly;
};

#endif
