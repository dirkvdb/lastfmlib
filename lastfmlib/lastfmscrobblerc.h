//    Copyright (C) 2007 Dirk Vanden Boer <dirk.vdb@gmail.com>
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

#ifndef LAST_FM_SCROBBLERC_H
#define LAST_FM_SCROBBLERC_H

#include <time.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct AVFormatContext;

typedef struct lastfm_scrobbler_struct
{
    char*                   username;               /* Last.fm username */
    char*                   password;               /* Last.fm password */
    int                     synchronous;            /* synchronous mode (default = 1) */
    int                     commit_only;            /* commit only mode (default = 0) */

    void*                   scrobbler;              /* for internal use only */
} lastfm_scrobbler;

typedef struct submission_info_struct
{
    char*           artist;               /**< \brief the artist */
    char*           track;                /**< \brief the track title */
    char*           album;                /**< \brief the album */

    wchar_t*        artist_wide;          /**< \brief the artist in widechar*/
    wchar_t*        track_wide;           /**< \brief the track title in widechar*/
    wchar_t*        album_wide;           /**< \brief the album in widechar*/

    int             track_length_in_secs; /**< \brief the track length (in seconds) */
    int             track_nr;             /**< \brief the track number */
    char*           music_brainz_id;      /**< \brief the Music Brainz Id */
    time_t          time_started;         /**< \brief time the track started (-1 for now, default -1) */
} submission_info;

/* create video_thumbnailer structure */
lastfm_scrobbler* create_scrobbler(const char* username, const char* password, int synchronous);
/* destroy video_thumbnailer structure */
void destroy_scrobbler(lastfm_scrobbler* scrobbler);

submission_info* create_submission_info();
void destroy_submission_info(submission_info* info);

void set_commit_only_mode(lastfm_scrobbler* scrobbler, int commit_only);

void started_playing(lastfm_scrobbler* scrobbler, const submission_info* info);
void finished_playing(lastfm_scrobbler* scrobbler);
void pause_playing(lastfm_scrobbler* scrobbler, int paused);

#ifdef __cplusplus
}
#endif

#endif
