#ifndef LAST_FM_EXCEPTIONS_H
#define LAST_FM_EXCEPTIONS_H

#include <stdexcept>

class ConnectionError : public std::logic_error
{
public:
    ConnectionError(const char* pMessage)
    : std::logic_error(pMessage)
    {
    }
};

#endif
