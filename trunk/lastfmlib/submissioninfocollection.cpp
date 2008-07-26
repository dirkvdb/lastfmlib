#include "submissioninfocollection.h"

#include <sstream>

using namespace std;

static const size_t MAX_QUEUE_SIZE = 50;

void SubmissionInfoCollection::addInfo(const SubmissionInfo& info)
{
    if (m_Infos.size() == MAX_QUEUE_SIZE)
    {
        m_Infos.pop_front();
    }
    m_Infos.push_back(info);
}

void SubmissionInfoCollection::clear()
{
    m_Infos.clear();
}

string SubmissionInfoCollection::getPostData() const
{
    stringstream ss;
    for (std::vector<SubmissionInfo>::size_type i = 0; i < m_Infos.size(); ++i)
    {
        ss << m_Infos[i].getPostData(static_cast<int>(i));
    }

    return ss.str();
}
