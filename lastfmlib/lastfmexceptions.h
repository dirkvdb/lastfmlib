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
 * @file lastfmexceptions.h
 * @brief Contains the definitions for exception classes used in lastfmlib
 * @author Dirk Vanden Boer
 */

#ifndef LAST_FM_EXCEPTIONS_H
#define LAST_FM_EXCEPTIONS_H

#include <stdexcept>
#include <string>

/** The ConnectionError class is an exception class that inherits from
 * std::exception. It is used to indicate that no connection could be
 * established.
 */
class ConnectionError : public std::exception
{
public:
    /** Constructor
     * \param message an std::string containing the error message
     */
    ConnectionError(const std::string& message) throw()
    : std::exception()
    , m_Message(message)
    {
    }

    /** Destructor
     */
    ~ConnectionError() throw()
    {
    }

    /** what
     * \return const char pointer containing an error message
     */
    const char* what() const throw()
    {
        return m_Message.c_str();
    }

private:
    std::string m_Message;
};

#endif
