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

#include "condition.h"

#include "mutex.h"

#include <cerrno>
#include <cstring>
#include <stdexcept>

#ifndef WIN32
#include <sys/time.h>
#endif

using namespace std;

namespace utils
{

Condition::Condition()
: m_Condition()
{
#ifndef WIN32
    if (pthread_cond_init(&m_Condition, NULL) != 0)
    {
        throw std::logic_error("Failed to create condition");
    }
#else
    m_Condition = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
}

Condition::~Condition()
{
#ifndef WIN32
    pthread_cond_destroy(&m_Condition);
#endif
}

void Condition::wait(Mutex& mutex)
{
#ifndef WIN32
    int ret = pthread_cond_wait(&m_Condition, mutex.getHandle());
    if (0 != ret)
    {
        throw std::logic_error(string("pthread_cond_wait returned: ") + strerror(ret));
    }
#else
    LeaveCriticalSection(mutex.getHandle());
    WaitForSingleObject(m_Condition, INFINITE);
    EnterCriticalSection(mutex.getHandle());
#endif
}

bool Condition::wait(Mutex& mutex, int32_t timeoutInMs)
{
#ifndef WIN32
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    struct timespec timeoutTime;
    uint64_t nanoSeconds = (static_cast<uint64_t>(timeoutInMs) * 1000000) + (static_cast<uint64_t>(currentTime.tv_usec) * 1000);
    int seconds = nanoSeconds / 1000000000;

    timeoutTime.tv_sec += currentTime.tv_sec + seconds;
    timeoutTime.tv_nsec = static_cast<long>(nanoSeconds % 1000000000);

    int ret = pthread_cond_timedwait(&m_Condition, mutex.getHandle(), &timeoutTime);
    if (ETIMEDOUT == ret)
    {
        return false;
    }
    else if (0 != ret)
    {
        throw std::logic_error(string("pthread_cond_timedwait returned: ") + strerror(ret));
    }
#else
    LeaveCriticalSection(mutex.getHandle());
    DWORD ret = WaitForSingleObject(m_Condition, timeoutInMs);
    if (ret == WAIT_TIMEOUT)
    {
        return false;
    }
    else if (ret == WAIT_FAILED)
    {
        throw std::logic_error("Failed to wait for mutex");
    }

    EnterCriticalSection(mutex.getHandle());
#endif

    return true;
}

void Condition::signal()
{
#ifndef WIN32
    pthread_cond_signal(&m_Condition);
#else
    SetEvent(m_Condition);
#endif
}

void Condition::broadcast()
{
#ifndef WIN32
    pthread_cond_broadcast(&m_Condition);
#else
    SetEvent(m_Condition);
#endif
}

}
