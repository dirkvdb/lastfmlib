#ifndef SUBMISSION_INFO_H
#define SUBMISSION_INFO_H

#include <ctime>
#include <vector>
#include <string>

#include "nowplayinginfo.h"

enum TrackSource
{
    UserChosen,
    NonPersonalizedBroadCast,
    PersonalizedRecommendation,
    Lastfm,
    Unknown
};

enum TrackRating
{
    Love,
    Ban,
    Skip,
    None
};


class SubmissionInfo : public NowPlayingInfo
{
public:
    SubmissionInfo();
    SubmissionInfo(const std::string& artist, const std::string& title, time_t timeStarted = -1);
    SubmissionInfo(const std::wstring& artist, const std::wstring& title, time_t timeStarted = -1);
    
    std::string getPostData(int index = 0) const;
    time_t getTimeStarted() const;

    void setSource(TrackSource source, const std::string& recommendationKey = "");
    void setRating(TrackRating rating);
    void setTimeStarted(time_t timeStarted);
    
private:
    time_t          m_TimeStarted;
    TrackSource     m_Source;
    TrackRating     m_Rating;
    std::string     m_RecommendationKey;
};

#endif
