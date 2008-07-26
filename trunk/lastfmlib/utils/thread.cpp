#include "thread.h"

#include <iostream>

Thread::Thread(ThreadFunction pfnThreadFunction, void* pInstance)
: m_Thread(0)
, m_pfnThreadFunction(pfnThreadFunction)
, m_pInstance(pInstance)
{
}

Thread::~Thread()
{
    if (m_Thread != 0)
    {
        pthread_detach(m_Thread);
    }
}

void Thread::start()
{
    pthread_create(&m_Thread, NULL, m_pfnThreadFunction, m_pInstance);
}

void Thread::join()
{
    if (m_Thread != 0)
    {
        pthread_join(m_Thread, NULL);
    }
}

void Thread::cancel()
{
    if (m_Thread != 0)
    {
        pthread_cancel(m_Thread);
    }
}
