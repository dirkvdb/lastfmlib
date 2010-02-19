//    Copyright (C) 2009 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "lastfmscrobblerc.h"
#include "lastfmscrobbler.h"

#include <iostream>
#include <stdexcept>
#include <cstring>

extern "C" lastfm_scrobbler* create_scrobbler(const char* username, const char* password, int32_t hashed_password, int32_t synchronous)
{
    lastfm_scrobbler* scrobbler = new lastfm_scrobbler();
    scrobbler->scrobbler    = new LastFmScrobbler(username, password, hashed_password == 0 ? false : true, synchronous == 0 ? false : true);
    return scrobbler;
}

extern "C" lastfm_scrobbler* create_identified_scrobbler(const char* client_identifier, const char* client_version, const char* username, const char* password, int32_t hashed_password, int32_t synchronous)
{
    lastfm_scrobbler* scrobbler = new lastfm_scrobbler();
    scrobbler->scrobbler    = new LastFmScrobbler(client_identifier, client_version, username, password, hashed_password == 0 ? false : true, synchronous == 0 ? false : true);
    return scrobbler;
}

extern "C" void destroy_scrobbler(lastfm_scrobbler* scrobbler)
{
    delete reinterpret_cast<LastFmScrobbler*>(scrobbler->scrobbler);
    delete scrobbler;
}

extern "C" void authenticate_scrobbler(lastfm_scrobbler* scrobbler)
{
	reinterpret_cast<LastFmScrobbler*>(scrobbler->scrobbler)->authenticate();
}

extern "C" void set_proxy_server(lastfm_scrobbler* scrobbler, const char* server, uint32_t port, const char* username, const char* password)
{
	reinterpret_cast<LastFmScrobbler*>(scrobbler->scrobbler)->setProxy(server, port, username ? username : "", password ? password : "");
}

extern "C" submission_info* create_submission_info()
{
    submission_info* info = new submission_info();

    info->artist                   = NULL;
    info->track                    = NULL;
    info->album                    = NULL;

    info->artist_wide              = NULL;
    info->track_wide               = NULL;
    info->album_wide               = NULL;

    info->track_length_in_secs     = 0;
    info->track_nr                 = -1;
    info->music_brainz_id          = NULL;
    info->time_started             = -1;

    info->track_source             = UserChosen;
    info->track_rating             = NoRating;
    info->recommendation_key       = NULL;

    return info;
}

extern "C" void destroy_submission_info(submission_info* info)
{
    delete info;
}

extern "C" void set_commit_only_mode(lastfm_scrobbler* scrobbler, int32_t commit_only)
{
    reinterpret_cast<LastFmScrobbler*>(scrobbler->scrobbler)->setCommitOnlyMode(commit_only != 0);
}

extern "C" void started_playing(lastfm_scrobbler* scrobbler, const submission_info* info)
{
    SubmissionInfo submissionInfo;
    submissionInfo.setTimeStarted(info->time_started);
    submissionInfo.setTrackLength(info->track_length_in_secs);
    submissionInfo.setTrackNr(info->track_nr);
    submissionInfo.setRating(info->track_rating);

    if (info->recommendation_key)
        submissionInfo.setSource(info->track_source, info->recommendation_key);
    else
        submissionInfo.setSource(info->track_source);

    if (info->artist_wide)
        submissionInfo.setArtist(info->artist_wide);
    else if (info->artist)
        submissionInfo.setArtist(info->artist);

    if (info->track_wide)
        submissionInfo.setTrack(info->track_wide);
    else if (info->track)
        submissionInfo.setTrack(info->track);

    if (info->album_wide)
        submissionInfo.setAlbum(info->album_wide);
    else if (info->album)
        submissionInfo.setAlbum(info->album);

    if (info->music_brainz_id)
        submissionInfo.setMusicBrainzId(info->music_brainz_id);

    reinterpret_cast<LastFmScrobbler*>(scrobbler->scrobbler)->startedPlaying(submissionInfo);
}

extern "C" void finished_playing(lastfm_scrobbler* scrobbler)
{
    reinterpret_cast<LastFmScrobbler*>(scrobbler->scrobbler)->finishedPlaying();
}

extern "C" void pause_playing(lastfm_scrobbler* scrobbler, int32_t paused)
{
    reinterpret_cast<LastFmScrobbler*>(scrobbler->scrobbler)->pausePlaying(paused != 0);
}

extern "C" char* create_password_hash(const char* password)
{
    return strdup(LastFmClient::generatePasswordHash(password).c_str());
}
