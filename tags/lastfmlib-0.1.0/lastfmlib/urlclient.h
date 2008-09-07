#ifndef URL_CLIENT_H
#define URL_CLIENT_H

#include <string>

class UrlClient
{
public:
    UrlClient();
    ~UrlClient();
    
    void get(const std::string& url, std::string& response);
    void getBinary(const std::string& url, void* callback, void* parameter);
    void post(const std::string& url, const std::string& data, std::string& response);
    
private:
    void initialize();
    void cleanup();
};

#endif
