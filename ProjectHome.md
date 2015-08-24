lastfmlib is library that provides an implementation of the Last.fm Submissions Protocol v1.2. It allows you to scrobble your tracks on Last.fm

## News ##
### Version 0.4.0 has been released (November 28, 2009) ###
  * Option added to override the systems proxy server (Library users must manually call the authenticate method before scrobbling tracks!)

### Version 0.3.1 has been released (October 26, 2009) ###
  * Changed library name to avoid name collision with other library

### Version 0.3.0 has been released (July 30, 2009) ###
  * Added option to specify your own client Last.fm client identier

### Version 0.2.1 has been released (April 29, 2009) ###
  * Fixed authentication response check to avoid authentication errors

### Version 0.2.0 has been released (March 23, 2009) ###
  * Interface change which adds option for hashed passwords

### Version 0.1.4 has been released (March 21, 2009) ###
  * Use syslog for logging in stead of log4cpp
  * Made inclusion guards more specific to avoid build issues

### Version 0.1.3 has been released (December 10, 2008) ###
  * Fixed logging not being enabled with --enable-logging
  * Fixed C interface not scrobbling tracks when time\_started was not explicitly set

### Version 0.1.2 has been released (November 18, 2008) ###
  * C interface for LastFmScrobbler added
  * Fixed submission of tracks that where not played long enough
  * Reauthenticate if Lastf.fm server returns BADSESSION

### Version 0.1.1 has been released (September 08, 2008) ###
  * Use gettimeofday() in stead of clock\_gettime()
  * Implemented asynchronous "finished playing"
  * Fixed installation location of header files

### Version 0.1.0 has been released (September 07, 2008) ###
  * Initial release