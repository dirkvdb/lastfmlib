#ifndef THERAD_H
#define THERAD_H

#include <pthread.h>

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

#endif
