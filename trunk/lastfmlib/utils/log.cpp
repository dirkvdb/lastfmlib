#include "log.h"

#ifdef ENABLE_LOGGING
    #include <log4cpp/BasicLayout.hh>
#endif

namespace utils
{

using namespace std;

Log::Log(const string& filename)
{
#ifdef ENABLE_LOGGING
    m_pAppender = new log4cpp::FileAppender("lastfmliblog", filename);
    m_pAppender->setLayout(new log4cpp::BasicLayout());

    m_pRootCategory = &log4cpp::Category::getRoot();
    m_pRootCategory->setAdditivity(false);
    m_pRootCategory->addAppender(m_pAppender);
#endif
}

Log::~Log()
{
#ifdef ENABLE_LOGGING
    log4cpp::Category::shutdown();
#endif
}

void Log::error(const std::string& message)
{
#ifdef ENABLE_LOGGING
    m_pRootCategory->error(message);
#endif
}

void Log::info(const std::string& message)
{
#ifdef ENABLE_LOGGING
    m_pRootCategory->info(message);
#endif
}

}
