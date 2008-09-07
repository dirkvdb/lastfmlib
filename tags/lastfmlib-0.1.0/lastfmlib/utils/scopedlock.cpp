#include "scopedlock.h"

#include "mutex.h"

namespace utils
{

ScopedLock::ScopedLock(Mutex& mutex)
: m_Mutex(mutex)
{
    m_Mutex.lock();
}

ScopedLock::~ScopedLock()
{
    m_Mutex.unlock();
}

}
