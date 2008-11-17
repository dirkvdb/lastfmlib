#include "lastfmclientmock.h"

#include <iostream>

using namespace std;

LastFmClientMock::LastFmClientMock()
: m_HandShakeThrowConnectionError(false)
, m_HandShakeThrowException(false)
, m_BadSessionError(false)
, m_NowPlayingCalled(false)
, m_SubmitCalled(false)
, m_SubmitCollectionCalled(false)
, m_HandshakeCalled(false)
{
}

void LastFmClientMock::handshake(const std::string& user, const std::string& pass)
{
    m_HandshakeCalled = true;

    if (m_HandShakeThrowConnectionError)
    {
        throw logic_error("connection error");
    }

    if (m_HandShakeThrowException)
    {
        throw logic_error("an exception");
    }
}

void LastFmClientMock::nowPlaying(const NowPlayingInfo& info)
{
    m_NowPlayingCalled = true;
    m_LastRecPlayingInfo = info;

    if (m_BadSessionError)
    {
        m_BadSessionError = false;
        throw BadSessionError("");
    }
}

void LastFmClientMock::submit(const SubmissionInfo& info)
{
    m_SubmitCalled = true;
    m_LastRecSubmitInfo = info;

    if (m_BadSessionError)
    {
        m_BadSessionError = false;
        throw BadSessionError("");
    }
}

void LastFmClientMock::submit(const SubmissionInfoCollection& infoCollection)
{
    m_SubmitCollectionCalled = true;
    m_LastRecSubmitInfoCollection = infoCollection;

    if (m_BadSessionError)
    {
        m_BadSessionError = false;
        throw BadSessionError("");
    }
}
