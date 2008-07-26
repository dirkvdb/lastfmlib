#ifndef SCOPED_LOCK_H
#define SCOPED_LOCK_H

class Mutex;

class ScopedLock
{
public:
    ScopedLock(Mutex& mutex);
    ~ScopedLock();

private:
    Mutex& m_Mutex;
};

#endif
