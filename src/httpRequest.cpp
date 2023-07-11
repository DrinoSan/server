

// Project Headers
#include "httpRequest.h"

//----------------------------------------------------------------------------
HttpRequest_t::HttpMethode
HttpRequest_t::stringToHttpMethode( std::string methode )
{
    if ( methode == "GET" )
        return HttpRequest_t::HttpMethode::GET;
    else if ( methode == "POST" )
        return HttpRequest_t::HttpMethode::POST;
    else if ( methode == "DELETE" )
        return HttpRequest_t::HttpMethode::DELETE;
    else if ( methode == "UPDATE" )
        return HttpRequest_t::HttpMethode::UPDATE;
    else if ( methode == "PUT" )
        return HttpRequest_t::HttpMethode::PUT;
    else if ( methode == "OPTIONS" )
        return HttpRequest_t::HttpMethode::OPTIONS;
    else
        return HttpRequest_t::HttpMethode::UNKNOWN;
}