#ifndef CONDITION_H
#define CONDITION_H

#include <pthread.h>

class Mutex;

class Condition
{
public:
    Condition();
    ~Condition();

    void wait(Mutex& mutex);
    bool wait(Mutex& mutex, int timeoutInMs);
    void signal();
    void broadcast();

private:
    pthread_cond_t m_Condition;
};

#endif
