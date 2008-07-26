#ifndef SUBMISSION_INFO_COLLECTION_H
#define SUBMISSION_INFO_COLLECTION_H

#include <deque>
#include <string>
#include "submissioninfo.h"

class SubmissionInfoCollection
{
public:
    void addInfo(const SubmissionInfo& info);
    void clear();
    std::string getPostData() const;

private:
    std::deque<SubmissionInfo> m_Infos;
};

#endif
