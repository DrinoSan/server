#include <iostream>
#include <sstream>
#include <string>
#include <utility>

// Project Headers
#include "parser.h"
#include "utils/logging.h"

void Parser_t::mapRequestHeaders( HttpRequest_t& httpRequest )
{
    std::istringstream stream{ httpRequest.buffer };

    std::string method, path, version;

    stream >> method >> path >> version;
    stream.get();
    stream.get();

    httpRequest.method  = httpRequest.stringToHttpMethode( method );
    httpRequest.path    = path;
    httpRequest.version = version;

    if ( httpRequest.method == HttpRequest_t::HttpMethode::UNKNOWN )
    {
        traceError( "We got UNKNOWN http method" );
    }

    int32_t keyEndPos   = 0;
    int32_t valueEndPos = 0;

    {
        // Scope to destroy tmp early
        std::string tmp{ httpRequest.buffer };
        if ( tmp.find( "\r\n\r\n" ) == std::string::npos )
        {
            traceError( "HttpMessage was not correctly ended\n%s" );
            return;
        }
    }

    for ( std::string line; std::getline( stream, line, '\n' ); )
    {
        keyEndPos = 0;
        if ( ( keyEndPos = line.find( ":" ), keyEndPos ) == std::string::npos )
        {
            traceError( "Could not find ':'" );
            break;
        }

        valueEndPos = 0;
        if ( ( valueEndPos = line.find( "\r" ) ) == std::string::npos )
        {
            traceError( "Could not find correctly ended value" );
            break;
        }

        std::string key( line.c_str(), line.c_str() + keyEndPos );

        std::string value( line.c_str() + keyEndPos + 2,
                           line.c_str() + valueEndPos );

        httpRequest.headers.insert( { key, value } );
    }

    traceInfo( "Parsed Headers:" );
    for ( const auto& [ key, val ] : httpRequest.headers )
    {
        traceInfo( "    Key: %s --> Value: %s", key.c_str(), val.c_str() );
    }
}