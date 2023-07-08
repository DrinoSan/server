#include "logging.h"

//-----------------------------------------------------------------------------
void traceInfo( const std::string format, ... )
{
    const std::string formattedString =
        static_cast<std::string>( InfoStr ) + format;

    va_list args;
    va_start( args, format );
    vprintf( formattedString.c_str(), args );
    va_end( args );
    printf( "\n" );
}

//-----------------------------------------------------------------------------
void traceError( const std::string format, ... )
{
    const std::string formattedString =
        static_cast<std::string>( ErrorStr ) + format;

    va_list args;
    va_start( args, format );
    vprintf( formattedString.c_str(), args );
    va_end( args );
    printf( "\n" );
}