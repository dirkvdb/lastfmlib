#include "condition.h"

#include "mutex.h"
#include <sys/time.h>
#include <errno.h>
#include <stdexcept>

using namespace std;

namespace utils
{

Condition::Condition()
{
    pthread_cond_init(&m_Condition, NULL);
}

Condition::~Condition()
{
    pthread_cond_destroy(&m_Condition);
}

void Condition::wait(Mutex& mutex)
{
    int ret = pthread_cond_wait(&m_Condition, mutex.getHandle());
    if (0 != ret)
    {
        throw std::logic_error(string("pthread_cond_wait returned: ") + strerror(ret));
    }
}

bool Condition::wait(Mutex& mutex, int timeoutInMs)
{
    struct timespec timeoutTime;
    clock_gettime(CLOCK_REALTIME, &timeoutTime);

    long long nanoSeconds = timeoutInMs * 1000 + timeoutTime.tv_sec;
    int seconds = nanoSeconds / 1000000000;

    timeoutTime.tv_sec += seconds;
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

    return true;
}

void Condition::signal()
{
    pthread_cond_signal(&m_Condition);
}

void Condition::broadcast()
{
    pthread_cond_broadcast(&m_Condition);
}

}
