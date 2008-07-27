#include "thread.h"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;


Thread::Thread(ThreadFunction pfnThreadFunction, void* pInstance)
: m_Thread(0)
, m_Key(0)
, m_pfnThreadFunction(pfnThreadFunction)
{
    int ret = pthread_key_create(&m_Key, Thread::onThreadExit);
    if (0 != ret)
    {
        throw logic_error(string("Failed to create thread key: ") + strerror(ret));
    }

    m_InstancePtrs.pThreadInstance = this;
    m_InstancePtrs.pRunInstance = pInstance;
}

Thread::~Thread()
{
    pthread_key_delete(m_Key);
}

void Thread::start()
{
    int ret = pthread_create(&m_Thread, NULL, Thread::onThreadStart, &m_InstancePtrs);
    if (0 != ret)
    {
        throw logic_error(string("Failed to create thread: ") + strerror(ret));
    }
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

bool Thread::isRunning()
{
    return m_Thread != 0;
}

void* Thread::onThreadStart(void* data)
{
    InstancePointers* pPtrs = reinterpret_cast<InstancePointers*>(data);

    int ret = pthread_setspecific(pPtrs->pThreadInstance->m_Key, pPtrs->pThreadInstance);
    if (0 != ret)
    {
        throw logic_error(string("Failed to set thread data: ") + strerror(ret));
    }
    return pPtrs->pThreadInstance->m_pfnThreadFunction(pPtrs->pRunInstance);
}

void Thread::onThreadExit(void* data)
{
    Thread* pThread = reinterpret_cast<Thread*>(data);
    pthread_detach(pThread->m_Thread);
    pThread->m_Thread = 0;
}
