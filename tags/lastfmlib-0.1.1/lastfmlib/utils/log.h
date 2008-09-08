#ifndef LOG_H
#define LOG_H

#ifdef ENABLE_LOGGING
    #include <log4cpp/Category.hh>
    #include <log4cpp/FileAppender.hh>
#endif

#include <string>

namespace utils
{
class Log
{
public:
    Log(const std::string& filename);
    ~Log();

    void error(const std::string& message);
    void info(const std::string& message);

private:
#ifdef ENABLE_LOGGING
    log4cpp::Appender*       m_pAppender;
    log4cpp::Category*       m_pRootCategory;
#endif
};

}

#endif
