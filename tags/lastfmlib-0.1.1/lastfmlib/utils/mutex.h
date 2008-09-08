#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

namespace utils
{

class Mutex
{
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();

    pthread_mutex_t* getHandle();

private:
    pthread_mutex_t m_Mutex;
};

}

#endif
