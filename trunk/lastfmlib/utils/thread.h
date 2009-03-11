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

#ifndef THERAD_H
#define THERAD_H

#include <pthread.h>

namespace utils
{
typedef void* (*ThreadFunction)(void* pInstance);

class Thread
{
public:
    Thread(ThreadFunction pfnThreadFunction, void* pInstance);
    ~Thread();

    void start();
    void join();
    void cancel();
    bool isRunning();

private:
    struct InstancePointers
    {
        Thread* pThreadInstance;
        void*   pRunInstance;
    };

    static void* onThreadStart(void* data);
    static void onThreadExit(void* data);

    pthread_t           m_Thread;
    pthread_key_t       m_Key;
    ThreadFunction      m_pfnThreadFunction;
    InstancePointers    m_InstancePtrs;
};
}

#endif