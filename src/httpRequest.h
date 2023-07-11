#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <cstdint>

// Project Headers
#include "httpMessage.h"

class HttpRequest_t : public HttpMessage_t
{
  public:
    enum class HttpMethode
    {
        GET,
        POST,
        DELETE,
        PUT,
        UPDATE,
        OPTIONS,
        UNKNOWN,
    };

  public:
    HttpMethode method;
    std::string path;
    std::string version;

    HttpMethode stringToHttpMethode( std::string method );
};

#endif   // HTTPREQUEST_H