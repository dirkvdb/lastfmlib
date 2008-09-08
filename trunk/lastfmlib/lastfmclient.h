/**
 * @file lastfmclient.h
 * @brief Contains the LastFmClient class
 * @author Dirk Vanden Boer
 */

#ifndef LAST_FM_CLIENT_H
#define LAST_FM_CLIENT_H

#include <string>
#include "urlclient.h"
#include "lastfmexceptions.h"

#include <iostream>

class NowPlayingInfo;
class SubmissionInfo;
class SubmissionInfoCollection;

/** The LastFmClient class provides access to Last.Fm to submit tracks
 *  and set Now Playing info.
 */
class LastFmClient
{
public:
    virtual ~LastFmClient() {}

    /** Attempt to authenticate with the Last.fm server
     * \param user an std::string containing the username
     * \param pass an std::string containing the password
     * \exception ConnectionError when connection to Last.fm server fails
     * \exception std::logic_error when authentication with Last.fm server fails
     */
    virtual void handshake(const std::string& user, const std::string& pass);

    /** Set a track as Now Playing on Last.fm
     * \param info a NowPlaying oject containing the current track information
     * \exception ConnectionError when connection to Last.fm server fails
     * \exception std::logic_error when setting Now Playing info fails
     */
    virtual void nowPlaying(const NowPlayingInfo& info);

    /** Submit a played track to the Last.fm server
     * \param info a SubmissionInfo oject containing information about the played track
     * \exception ConnectionError when connection to Last.fm server fails
     * \exception std::logic_error when submitting the Track info fails
     */
    virtual void submit(const SubmissionInfo& info);

    /** Submit a collection of played tracks to the Last.fm server (max. 50)
     * \param infoCollection a SubmissionInfoCollection oject containing played tracks
     * \exception ConnectionError when connection to Last.fm server fails
     * \exception std::logic_error when submitting the Track info collection fails
     */
    virtual void submit(const SubmissionInfoCollection& infoCollection);

private:
    std::string createRequestString(const std::string& user, const std::string& pass);
    std::string createNowPlayingString(const NowPlayingInfo& info);
    std::string createSubmissionString(const SubmissionInfo& info);
    std::string createSubmissionString(const SubmissionInfoCollection& infoCollection);
    void throwOnInvalidSession();
    void submit(const std::string& postData);

    UrlClient       m_UrlClient;
    std::string     m_SessionId;
    std::string     m_NowPlayingUrl;
    std::string     m_SubmissionUrl;
};

#endif
