#ifndef LAST_FM_EXCEPTIONS_H
#define LAST_FM_EXCEPTIONS_H

#include <stdexcept>
#include <string>

/** The ConnectionError class is an exception class that inherits from
 * std::exception. It is used to indicate that no connection could be
 * established.
 */
class ConnectionError : public std::exception
{
public:
    /** Constructor
     * \param message an std::string containing the error message
     */
    ConnectionError(const std::string& message) throw()
    : std::exception()
    , m_Message(message)
    {
    }

    /** Destructor
     */
    ~ConnectionError() throw()
    {
    }

    /** what
     * \return const char pointer containing an error message
     */
    const char* what() const throw()
    {
        return m_Message.c_str();
    }

private:
    std::string m_Message;
};

#endif
