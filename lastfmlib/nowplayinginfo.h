#ifndef NOW_PLAYING_INFO_H
#define NOW_PLAYING_INFO_H

#include <string>
#include <iostream>


class NowPlayingInfo
{
public:
    NowPlayingInfo();
    NowPlayingInfo(const std::string& artist, const std::string& track);
    NowPlayingInfo(const std::wstring& artist, const std::wstring& track);
    std::string getPostData() const;

    void setAlbum(const std::string& album);
    void setAlbum(const std::wstring& album);

    void setTrackLength(int lengthInSecs);
    void setTrackNr(int trackNr);
    void setMusicBrainzId(const std::string& musicBrainzId);
    void setMusicBrainzId(const std::wstring& musicBrainzId);

    const std::string& getArtist() const;
    const std::string& getTrack() const;
    const std::string& getAlbum() const;
    int getTrackLength() const;

protected:
    std::string     m_Artist;
    std::string     m_Track;
    std::string     m_Album;

    int             m_TrackLengthInSecs;
    int             m_TrackNr;
    std::string     m_MusicBrainzId;
};

#endif
