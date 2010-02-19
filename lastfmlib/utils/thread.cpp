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

#include "thread.h"

#include <stdexcept>
#include <string>
#include <cstring>

#include "types.h"

namespace utils
{

using namespace std;


Thread::Thread(ThreadFunction pfnThreadFunction, void* pInstance)
: m_Thread(0)
#ifndef WIN32
, m_Key(0)
#else
, m_ThreadId(0)
#endif
, m_pfnThreadFunction(pfnThreadFunction)
, m_InstancePtrs()
{
#ifndef WIN32
    int32_t ret = pthread_key_create(&m_Key, Thread::onThreadExit);
    if (0 != ret)
    {
        throw logic_error(string("Failed to create thread key: ") + strerror(ret));
    }
#endif

    m_InstancePtrs.pThreadInstance = this;
    m_InstancePtrs.pRunInstance = pInstance;
}

Thread::~Thread()
{
#ifndef WIN32
    pthread_key_delete(m_Key);
#endif
}

void Thread::start()
{
#ifndef WIN32
    int32_t ret = pthread_create(&m_Thread, NULL, Thread::onThreadStart, &m_InstancePtrs);
    if (0 != ret)
    {
        throw logic_error(string("Failed to create thread: ") + strerror(ret));
    }
#else
    m_Thread = CreateThread(NULL, 0, Thread::winThreadFunc, &m_InstancePtrs, 0, &m_ThreadId);
#endif
}

void Thread::join()
{
    if (m_Thread != 0)
    {
#ifndef WIN32
        pthread_join(m_Thread, NULL);
#else
        WaitForSingleObject(m_Thread, INFINITE);
#endif
    }
}

void Thread::cancel()
{
    if (m_Thread != 0)
    {
#ifndef WIN32
        pthread_cancel(m_Thread);
#else
        TerminateThread(m_Thread, 1);
#endif
    }
}

bool Thread::isRunning()
{
    return m_Thread != 0;
}

#ifndef WIN32
void* Thread::onThreadStart(void* data)
{
    InstancePointers* pPtrs = reinterpret_cast<InstancePointers*>(data);

    int32_t ret = pthread_setspecific(pPtrs->pThreadInstance->m_Key, pPtrs->pThreadInstance);
    if (0 != ret)
    {
        throw logic_error(string("Failed to set thread data: ") + strerror(ret));
    }
    return pPtrs->pThreadInstance->m_pfnThreadFunction(pPtrs->pRunInstance);
}

void Thread::onThreadExit(void* data)
{
    Thread* pThread = reinterpret_cast<Thread*>(data);
    //pthread_detach(pThread->m_Thread);
    pThread->m_Thread = 0;
}
#else
DWORD WINAPI Thread::winThreadFunc(LPVOID pData)
{
    InstancePointers* pPtrs = reinterpret_cast<InstancePointers*>(pData);
    pPtrs->pThreadInstance->m_pfnThreadFunction(pPtrs->pRunInstance);

    return 0;
}
#endif

}
