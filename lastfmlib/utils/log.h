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

#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#ifdef ENABLE_LOGGING
    #include <log4cpp/Category.hh>
    #include <log4cpp/FileAppender.hh>
#endif

#include <string>

namespace utils
{
class Log
{
public:
    Log(const std::string& filename);
    ~Log();

    void error(const std::string& message);
    void info(const std::string& message);

private:
#ifdef ENABLE_LOGGING
    log4cpp::Appender*       m_pAppender;
    log4cpp::Category*       m_pRootCategory;
#endif
};

}

#endif
