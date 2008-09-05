#include "lastfmclient.h"

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>

#include "md5/md5.h"
#include "utils/stringoperations.h"

#include "nowplayinginfo.h"
#include "submissioninfo.h"
#include "submissioninfocollection.h"

using namespace std;
using namespace StringOperations;

static const string CLIENT_IDENTIFIER = "lfc";
static const string CLIENT_VERSION = "1.0";

void LastFmClient::handshake(const string& user, const string& pass)
{
    string response;
    try
    {
        m_UrlClient.get(createRequestString(user, pass), response);
    }
    catch (logic_error& e)
    {
        throw ConnectionError(e.what());
    }

    vector<string> lines = tokenize(response, "\n");
    if (lines[0] != "OK")
    {
        throw logic_error("Failed to connect to last.fm: " + lines[0]);
    }
    else if (lines.size() != 5)
    {
        throw logic_error("Failed to connect to last.fm: invalid response length");
    }

    m_SessionId = lines[1];
    m_NowPlayingUrl = lines[2];
    m_SubmissionUrl = lines[3];
}

void LastFmClient::nowPlaying(const NowPlayingInfo& info)
{
    throwOnInvalidSession();

    string response;
    try
    {
        m_UrlClient.post(m_NowPlayingUrl, createNowPlayingString(info), response);
    }
    catch (logic_error& e)
    {
        throw ConnectionError(e.what());
    }

    vector<string> lines = tokenize(response, "\n");
    if (lines[0] != "OK")
    {
        throw logic_error("Failed to set now playing info: " + lines[0]);
    }
}

void LastFmClient::submit(const SubmissionInfo& info)
{
    submit(createSubmissionString(info));

}

void LastFmClient::submit(const SubmissionInfoCollection& infoCollection)
{
    submit(createSubmissionString(infoCollection));
}

void LastFmClient::submit(const string& postData)
{
    throwOnInvalidSession();

    string response;

    try
    {
        m_UrlClient.post(m_SubmissionUrl, postData, response);
    }
    catch (logic_error& e)
    {
        throw ConnectionError(e.what());
    }

    vector<string> lines = tokenize(response, "\n");
    if (lines[0] != "OK")
    {
        throw logic_error("Failed to submit info: " + lines[0]);
    }
}


string generateMD5String(const string& data)
{
    md5_byte_t digest[16];

    md5_state_t state;
    md5_init(&state);
    md5_append(&state, reinterpret_cast<const md5_byte_t*>(data.c_str()), data.size());
    md5_finish(&state, digest);

    stringstream md5String;
    for (int i = 0; i < 16; ++i)
    {
        md5String << setw(2) << setfill('0') << hex << static_cast<int>(digest[i]);
    }

    return md5String.str();
}

string generateAutenticationToken(const string& pass, time_t timestamp)
{
    return generateMD5String(generateMD5String(pass) + getPostData(timestamp));
}

string LastFmClient::createRequestString(const string& user, const string& pass)
{
    time_t timestamp = time(0);

    stringstream request;
    request << "http://post.audioscrobbler.com/?hs=true&p=1.2"
            << "&c=" << CLIENT_IDENTIFIER
            << "&v=" << CLIENT_VERSION
            << "&u=" << user
            << "&t=" << timestamp
            << "&a=" << generateAutenticationToken(pass, timestamp);

    return request.str();
}

string LastFmClient::createNowPlayingString(const NowPlayingInfo& info)
{
    stringstream nowPlaying;
    nowPlaying << "&s=" << m_SessionId << info.getPostData();
    return nowPlaying.str();
}

string LastFmClient::createSubmissionString(const SubmissionInfo& info)
{
    stringstream submission;
    submission << "&s=" << m_SessionId << info.getPostData();
    return submission.str();
}

string LastFmClient::createSubmissionString(const SubmissionInfoCollection& infoCollection)
{
    stringstream submission;
    submission << "&s=" << m_SessionId << infoCollection.getPostData();
    return submission.str();
}

void LastFmClient::throwOnInvalidSession()
{
    if (m_SessionId.empty())
    {
        throw logic_error("No last.fm session available");
    }
}
