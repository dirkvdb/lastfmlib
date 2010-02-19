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

#include "mutex.h"

#include <stdexcept>

namespace utils
{

Mutex::Mutex()
: m_Mutex()
{
#ifndef WIN32
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_Mutex, &mutexAttr);
#else
    InitializeCriticalSection(&m_Mutex);
#endif
}

Mutex::~Mutex()
{
#ifndef WIN32
    pthread_mutex_destroy(&m_Mutex);
#else
    DeleteCriticalSection(&m_Mutex);
#endif
}

void Mutex::lock()
{
#ifndef WIN32
    pthread_mutex_lock(&m_Mutex);
#else
    EnterCriticalSection(&m_Mutex);
#endif
}

void Mutex::unlock()
{
#ifndef WIN32
    pthread_mutex_unlock(&m_Mutex);
#else
    LeaveCriticalSection(&m_Mutex);
#endif
}

MutexHandle* Mutex::getHandle()
{
    return &m_Mutex;
}

}
