#include "mutex.h"

Mutex::Mutex()
{
    pthread_mutex_init(&m_Mutex, NULL);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_Mutex);
}

void Mutex::lock()
{
    pthread_mutex_lock(&m_Mutex);
}

void Mutex::unlock()
{
    pthread_mutex_unlock(&m_Mutex);
}

pthread_mutex_t* Mutex::getHandle()
{
    return &m_Mutex;
}
