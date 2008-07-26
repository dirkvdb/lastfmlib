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

private:
    pthread_t       m_Thread;
    ThreadFunction  m_pfnThreadFunction;
    void*           m_pInstance;
};

#endif
