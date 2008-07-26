#ifndef LAST_FM_CLIENT_H
#define LAST_FM_CLIENT_H

#include <string>
#include "urlclient.h"
#include "lastfmexceptions.h"

#include <iostream>

class NowPlayingInfo;
class SubmissionInfo;
class SubmissionInfoCollection;

class LastFmClient
{
public:
    virtual void handshake(const std::string& user, const std::string& pass);
    virtual void nowPlaying(const NowPlayingInfo& info);
    virtual void submit(const SubmissionInfo& info);
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
