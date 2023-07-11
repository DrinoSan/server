#ifndef PARSER_H
#define PARSER_H

#include "httpRequest.h"

class Parser_t
{
  public:
    Parser_t() = default;
    ~Parser_t() = default;

    void mapRequestHeaders( HttpRequest_t& httpRequest );
};

#endif   // PARSER_H