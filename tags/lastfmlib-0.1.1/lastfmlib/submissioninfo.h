#ifndef SUBMISSION_INFO_H
#define SUBMISSION_INFO_H

#include <ctime>
#include <vector>
#include <string>

#include "nowplayinginfo.h"

/** The NowPlayingInfo class contains all the necessary information to
 *  submit a played track to Last.Fm. Artist, Track and StartTime are
 *  required fields that must be set. The class inherits from
 *  NowPlayingInfo and contains some additional information that could
 *  be needed to submit a track.
 */
class SubmissionInfo : public NowPlayingInfo
{
public:
    /** The TrackSource enum contains possible sources fo submitted tracks.
     * Note: Currently Last.fm only supports UserChosen and Lastfm
     */
    enum TrackSource
    {
        UserChosen,                 /**< Chosen by the user */
        NonPersonalizedBroadCast,   /**< Non-personalised broadcast (e.g. Shoutcast, BBC Radio 1) */
        PersonalizedRecommendation, /**< Personalised recommendation except Last.fm (e.g. Pandora, Launchcast) */
        Lastfm,                     /**< Last.fm */
        Unknown                     /**< Unknown source */
    };

    /** The TrackRating enum contains possible ratings for a track.
     */
    enum TrackRating
    {
        Love,   /**< On any mode if the user has manually loved the track. This implies a listen */
        Ban,    /**< Only if source = Lastfm. This implies a skip, and the client should skip to the next track when a ban happens */
        Skip,   /**< Only if source = Lastfm */
        None    /**< If no rating is applicable */
    };

    /** \brief Default constructor */
    SubmissionInfo();
    /** \brief Constructor that sets artist ,track and optionally the time the track started playing */
    SubmissionInfo(const std::string& artist, const std::string& title, time_t timeStarted = -1);
    /** \brief Constructor that sets artist ,track (unicode) and optionally the time the track started playing */
    SubmissionInfo(const std::wstring& artist, const std::wstring& title, time_t timeStarted = -1);

    /** \brief returns the postdata needed to submit the info to Last.fm, used by LastFmClient */
    std::string getPostData(int index = 0) const;
    /** \brief returns the time track started playing */
    time_t getTimeStarted() const;

    /** Set the source of the track
     * \param source the source type
     * \param recommendationKey The 5-digit Last.fm recommendation key.
     * Only needs to be supplied when the source is set to Lastfm.
     */
    void setSource(TrackSource source, const std::string& recommendationKey = "");
    /** Set the rating of the track
     * \param rating the track rating
     */
    void setRating(TrackRating rating);
    /** Set the time when the track started playing
     * \param timeStarted The time in UNIX timestamp format
     * (integer number of seconds since 00:00:00, January 1st 1970 UTC).
     * This must be in the UTC time zone.
     */
    void setTimeStarted(time_t timeStarted);

private:
    time_t          m_TimeStarted;
    TrackSource     m_Source;
    TrackRating     m_Rating;
    std::string     m_RecommendationKey;
};

#endif