#include "scopedlock.h"

#include "mutex.h"

ScopedLock::ScopedLock(Mutex& mutex)
: m_Mutex(mutex)
{
    m_Mutex.lock();
}

ScopedLock::~ScopedLock()
{
    m_Mutex.unlock();
}
