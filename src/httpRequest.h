#ifndef SERVER_HTTPREQUEST_H
#define SERVER_HTTPREQUEST_H

#include <cstdint>

// Project Headers
#include "httpMessage.h"


constexpr int32_t BUFFER_SIZE = 3000;

class HttpRequest_t : public HttpMessage_t
{
    public:
    char buffer[BUFFER_SIZE];
};

#endif // SERVER_HTTPREQUEST_H