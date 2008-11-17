#ifndef LAST_FM_CLIENT_MOCK_H
#define LAST_FM_CLIENT_MOCK_H

#include <string>
#include "lastfmlib/lastfmclient.h"
#include "lastfmlib/submissioninfo.h"
#include "lastfmlib/submissioninfocollection.h"

class NowPlayingInfo;
class SubmissionInfo;
class SubmissionInfoCollection;

class LastFmClientMock : public LastFmClient
{
public:
    LastFmClientMock();

    void handshake(const std::string& user, const std::string& pass);
    void nowPlaying(const NowPlayingInfo& info);
    void submit(const SubmissionInfo& info);
    void submit(const SubmissionInfoCollection& infoCollection);

    bool m_HandShakeThrowConnectionError;
    bool m_HandShakeThrowException;
    bool m_BadSessionError;
    bool m_NowPlayingCalled;
    bool m_SubmitCalled;
    bool m_SubmitCollectionCalled;
    bool m_HandshakeCalled;

    NowPlayingInfo m_LastRecPlayingInfo;
    SubmissionInfo m_LastRecSubmitInfo;
    SubmissionInfoCollection m_LastRecSubmitInfoCollection;
};

#endif
