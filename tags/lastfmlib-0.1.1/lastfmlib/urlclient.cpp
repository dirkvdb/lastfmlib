#include "urlclient.h"

#include <curl/curl.h>
#include <assert.h>
#include <stdexcept>

using namespace std;

size_t receiveData(char* data, size_t size, size_t nmemb, string* pBuffer);

UrlClient::UrlClient()
{
    initialize();
}

UrlClient::~UrlClient()
{
    cleanup();
}

void UrlClient::initialize()
{
#ifdef WIN32
    CURLcode rc = curl_global_init(CURL_GLOBAL_WIN32 | CURL_GLOBAL_ALL);
#else
    CURLcode rc = curl_global_init(CURL_GLOBAL_ALL);
#endif

    if (CURLE_OK != rc)
    {
        throw std::logic_error("Failed to initialize libcurl");
    }
}

void UrlClient::cleanup()
{
    curl_global_cleanup();
}

void UrlClient::get(const string& url, string& response)
{
    CURL* curlHandle = curl_easy_init();
    assert(curlHandle);

    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, receiveData);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curlHandle, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1);

    CURLcode rc = curl_easy_perform(curlHandle);
    curl_easy_cleanup(curlHandle);

    if (CURLE_OK != rc)
    {
        throw std::logic_error("Failed to get " + url + ": " + curl_easy_strerror(rc));
    }
}

void UrlClient::getBinary(const string& url, void* callback, void* parameter)
{
    CURL* curlHandle = curl_easy_init();
    assert(curlHandle);

    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, parameter);
    curl_easy_setopt(curlHandle, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1);

    CURLcode rc = curl_easy_perform(curlHandle);
    curl_easy_cleanup(curlHandle);

    if (CURLE_OK != rc)
    {
        throw std::logic_error("Failed to get " + url + ": " + curl_easy_strerror(rc));
    }
}

void UrlClient::post(const string& url, const string& data, string& response)
{
    CURL* curlHandle = curl_easy_init();
    assert(curlHandle);

    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, receiveData);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curlHandle, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1);

    CURLcode rc = curl_easy_perform(curlHandle);
    curl_easy_cleanup(curlHandle);

    if(CURLE_OK != rc)
    {
        throw std::logic_error("Failed to post " + url + ": " + curl_easy_strerror(rc));
    }
}

size_t receiveData(char* data, size_t size, size_t nmemb, string* pBuffer)
{
    assert(pBuffer);
    size_t dataSize = size * nmemb;
    pBuffer->append(data, dataSize);

    return dataSize;
}
