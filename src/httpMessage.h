#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#include <map>
#include <string>

constexpr int32_t BUFFER_SIZE = 3000;

class HttpMessage_t
{
    // TODO: Implement me
  public:
    std::map<std::string, std::string> headers;
    char                               buffer[ BUFFER_SIZE ];
};

#endif   // HTTPMESSAGE_H