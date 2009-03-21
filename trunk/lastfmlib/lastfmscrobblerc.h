//    Copyright (C) 2008 Dirk Vanden Boer <dirk.vdb@gmail.com>
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

/**
 * @file lastfmscrobblerc.h
 * @brief Contains the c interface for the LastFmScrobbler class
 * @author Dirk Vanden Boer
 */

#ifndef LAST_FM_SCROBBLERC_H
#define LAST_FM_SCROBBLERC_H

#include <time.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "lastfmtypes.h"

struct AVFormatContext;

typedef struct lastfm_scrobbler_struct
{
    void*                   scrobbler;              /**< \brief for internal use only */
} lastfm_scrobbler; /**< \brief struct containing info about the current Last.fm connection */

typedef struct submission_info_struct
{
    char*               artist;               /**< \brief the artist */
    char*               track;                /**< \brief the track title */
    char*               album;                /**< \brief the album */

    wchar_t*            artist_wide;          /**< \brief the artist in widechar*/
    wchar_t*            track_wide;           /**< \brief the track title in widechar*/
    wchar_t*            album_wide;           /**< \brief the album in widechar*/

    int                 track_length_in_secs; /**< \brief the track length (in seconds) */
    int                 track_nr;             /**< \brief the track number */
    char*               music_brainz_id;      /**< \brief the Music Brainz Id */
    time_t              time_started;         /**< \brief time the track started (-1 for current time, default -1) */

    enum TrackSource    track_source;         /**< \brief time the track source (default UserChosen) */
    enum TrackRating    track_rating;         /**< \brief time the track started (default None) */
    char*               recommendation_key;   /**< \brief The 5-digit Last.fm recommendation key needed when track_source is set to LastFm (default NULL) */
} submission_info; /**< \brief struct containing info about the song to commit */

/** Create Last.fm scrobbler struct
 * \param username Last.fm username
 * \param password Last.fm password
 * \param synchronous If 0 all functions will be executed in
 * a thread and return immediately (prevents long blocking methods in
 * case of network problems)
 * \return lastfm_scrobbler structure
 */
lastfm_scrobbler* create_scrobbler(const char* username, const char* password, int synchronous);

/** Destroy the Last.fm scrobbler struct
 * \param scrobbler structure to destroy
 */
void destroy_scrobbler(lastfm_scrobbler* scrobbler);

/** Create and initialize a submission_info struct
 * \return intialized submission info struct
 */
submission_info* create_submission_info();

/** Destroy a submission_info struct
 * \param info submission info struct to destroy
 */
void destroy_submission_info(submission_info* info);

/** When commit only mode is set, now playinginfo is not updated on
 * Last.fm, tracks will only be commited once they finished playing
 * \param scrobbler initialized scrobbler struct
 * \param commit_only set commitOnlyMode to 1 or 0
 */
void set_commit_only_mode(lastfm_scrobbler* scrobbler, int commit_only);

/** Indicate that a new track has started playing, the previous track
 * will be submitted (if available) and the new track will be set as
 * Now Playing
 * \param scrobbler initialized scrobbler struct
 * \param info submission_info struct containing information about
 * the new song
 */
void started_playing(lastfm_scrobbler* scrobbler, const submission_info* info);

/** Indicate that the current track has stopped playing. The current
 * track will be submitted to Last.fm
 */
void finished_playing(lastfm_scrobbler* scrobbler);

/** Indicate that playback of the current track has been (un)paused
 * \param scrobbler initialized scrobbler struct
 * \param paused 1 if track is being paused, 0 if being unpaused
 */
void pause_playing(lastfm_scrobbler* scrobbler, int paused);

#ifdef __cplusplus
}
#endif

#endif