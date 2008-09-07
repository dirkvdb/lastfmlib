#include "submissioninfo.h"
#include "utils/stringoperations.h"

#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace std;

std::string sourceToString(SubmissionInfo::TrackSource source, const std::string& recommendationKey)
{
    switch (source)
    {
        case SubmissionInfo::UserChosen:
            return "P";
            break;
        case SubmissionInfo::NonPersonalizedBroadCast:
            return "R";
            break;
        case SubmissionInfo::PersonalizedRecommendation:
            return "E";
            break;
        case SubmissionInfo::Lastfm:
            return "L" + recommendationKey;
            break;
        case SubmissionInfo::Unknown:
        default:
            return "U";
    }
}

std::string ratingToString(SubmissionInfo::TrackRating rating)
{
    switch (rating)
    {
        case SubmissionInfo::Love:
            return "L";
            break;
        case SubmissionInfo::Ban:
            return "B";
            break;
        case SubmissionInfo::Skip:
            return "S";
            break;
        case SubmissionInfo::None:
        default:
            return "";
    }
}

SubmissionInfo::SubmissionInfo()
: NowPlayingInfo()
, m_TimeStarted(0)
, m_Source(UserChosen)
, m_Rating(None)
{
}

SubmissionInfo::SubmissionInfo(const string& artist, const string& track, time_t timeStarted)
: NowPlayingInfo(artist, track)
, m_TimeStarted(timeStarted)
, m_Source(UserChosen)
, m_Rating(None)
{
}

SubmissionInfo::SubmissionInfo(const wstring& artist, const wstring& track, time_t timeStarted)
: NowPlayingInfo(artist, track)
, m_TimeStarted(timeStarted)
, m_Source(UserChosen)
, m_Rating(None)
{
}

string SubmissionInfo::getPostData(int index) const
{
    if (m_Source == UserChosen && m_TrackLengthInSecs < 0)
    {
        throw logic_error("Tracklength is required when submitting user chosen track");
    }

    stringstream ss;
    ss  << "&a[" << index << "]=" << StringOperations::urlEncode(m_Artist)
        << "&t[" << index << "]=" << StringOperations::urlEncode(m_Track)
        << "&i[" << index << "]=" << m_TimeStarted
        << "&o[" << index << "]=" << sourceToString(m_Source, m_RecommendationKey)
        << "&r[" << index << "]=" << ratingToString(m_Rating)
        << "&l[" << index << "]=" << (m_TrackLengthInSecs > 0 ? StringOperations::getPostData(m_TrackLengthInSecs) : "")
        << "&b[" << index << "]=" << StringOperations::urlEncode(m_Album)
        << "&n[" << index << "]=" << (m_TrackNr > 0 ? StringOperations::getPostData(m_TrackNr) : "")
        << "&m[" << index << "]=" << StringOperations::urlEncode(m_MusicBrainzId);

    return ss.str();
}

time_t SubmissionInfo::getTimeStarted() const
{
    return m_TimeStarted;
}

void SubmissionInfo::setSource(TrackSource source, const std::string& recommendationKey)
{
    m_Source = source;
    m_RecommendationKey = recommendationKey;
}

void SubmissionInfo::setRating(TrackRating rating)
{
    m_Rating = rating;
}

void SubmissionInfo::setTimeStarted(time_t timeStarted)
{
    m_TimeStarted = timeStarted;
}
