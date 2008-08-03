#ifndef NOW_PLAYING_INFO_H
#define NOW_PLAYING_INFO_H

#include <string>
#include <iostream>

/** The NowPlayingInfo class contains all the necessary information to
 *  set the Now Playing info on Last.Fm. Artist and Track are required
 *  fields that must be set.
 */
class NowPlayingInfo
{
public:
    /** \brief Default constructor */
    NowPlayingInfo();
    /** \brief Constructor that sets artist and track */
    NowPlayingInfo(const std::string& artist, const std::string& track);
    /** \brief Constructor that sets artist and track (unicode) */
    NowPlayingInfo(const std::wstring& artist, const std::wstring& track);

    /** \brief returns the postdata needed to submit the info to Last.fm, used by LastFmClient */
    std::string getPostData() const;

    /** \brief sets the album of the track */
    void setAlbum(const std::string& album);
    /** \brief sets the album of the track (unicode) */
    void setAlbum(const std::wstring& album);
    /** \brief sets the track length (in seconds) */
    void setTrackLength(int lengthInSecs);
    /** \brief sets the track number */
    void setTrackNr(int trackNr);
    /** \brief sets the Music Brainz Id */
    void setMusicBrainzId(const std::string& musicBrainzId);
    /** \brief sets the Music Brainz Id (unicode) */
    void setMusicBrainzId(const std::wstring& musicBrainzId);

    /** \brief returns the artist */
    const std::string& getArtist() const;
    /** \brief returns the track title */
    const std::string& getTrack() const;
    /** \brief returns the album */
    const std::string& getAlbum() const;
    /** \brief returns the track length (in seconds) */
    int getTrackLength() const;
    /** \brief returns the track number */
    int getTrackNr() const;
    /** \brief returns Music Brainz Id */
    const std::string& getMusicBrainzId();

protected:
    std::string     m_Artist;               /**< \brief the artist */
    std::string     m_Track;                /**< \brief the track title */
    std::string     m_Album;                /**< \brief the album */

    int             m_TrackLengthInSecs;    /**< \brief the track length (in seconds) */
    int             m_TrackNr;              /**< \brief the track number */
    std::string     m_MusicBrainzId;        /**< \brief the Music Brainz Id */
};

#endif
