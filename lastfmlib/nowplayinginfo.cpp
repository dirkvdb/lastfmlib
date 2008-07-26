#include "nowplayinginfo.h"

#include "utils/stringoperations.h"

using namespace std;

NowPlayingInfo::NowPlayingInfo()
: m_TrackLengthInSecs(-1)
, m_TrackNr(-1)
{
}

NowPlayingInfo::NowPlayingInfo(const std::string& artist, const std::string& track)
: m_Artist(artist)
, m_Track(track)
, m_TrackLengthInSecs(-1)
, m_TrackNr(-1)
{
}

NowPlayingInfo::NowPlayingInfo(const std::wstring& artist, const std::wstring& track)
: m_TrackLengthInSecs(-1)
, m_TrackNr(-1)
{
    StringOperations::wideCharToUtf8(artist, m_Artist);
    StringOperations::wideCharToUtf8(track, m_Track);
}

string NowPlayingInfo::getPostData() const
{
    stringstream ss;
    ss  << "&a=" << StringOperations::urlEncode(m_Artist)
        << "&t=" << StringOperations::urlEncode(m_Track)
        << "&b=" << StringOperations::urlEncode(m_Album)
        << "&l=" << (m_TrackLengthInSecs > 0 ? StringOperations::getPostData(m_TrackLengthInSecs) : "")
        << "&n=" << (m_TrackNr > 0 ? StringOperations::getPostData(m_TrackNr) : "")
        << "&m=" << StringOperations::urlEncode(m_MusicBrainzId);

    return ss.str();
}

void NowPlayingInfo::setAlbum(const std::string& album)
{
    m_Album = album;
}

void NowPlayingInfo::setAlbum(const std::wstring& album)
{
    StringOperations::wideCharToUtf8(album, m_Album);
}

void NowPlayingInfo::setTrackLength(int lengthInSecs)
{
    m_TrackLengthInSecs = lengthInSecs;
}

void NowPlayingInfo::setTrackNr(int trackNr)
{
    m_TrackNr = trackNr;
}

void NowPlayingInfo::setMusicBrainzId(const std::string& musicBrainzId)
{
    m_MusicBrainzId = musicBrainzId;
}

void NowPlayingInfo::setMusicBrainzId(const std::wstring& musicBrainzId)
{
    StringOperations::wideCharToUtf8(musicBrainzId, m_MusicBrainzId);
}

const std::string& NowPlayingInfo::getArtist() const
{
    return m_Artist;
}

const std::string& NowPlayingInfo::getTrack() const
{
    return m_Track;
}

const std::string& NowPlayingInfo::getAlbum() const
{
    return m_Album;
}

int NowPlayingInfo::getTrackLength() const
{
    return m_TrackLengthInSecs;
}
