#include "logging.h"
#include <iostream>
#include <string>

//-----------------------------------------------------------------------------
void LogInfo( int line, const char* file, const std::string format, ... )
{
    const std::string formattedString =
        static_cast<std::string>( InfoStr ) + format + "\t\t[ " +
        std::to_string( line ) + " ]" + "\t\t[" + std::string( file ) + "]";

    va_list args;
    va_start( args, format );
    vprintf( formattedString.c_str(), args );
    va_end( args );
    printf( "\n" );
}

//-----------------------------------------------------------------------------
void LogError( int line, const char* file, const std::string format, ... )
{
    const std::string formattedString =
        static_cast<std::string>( ErrorStr ) + format + "\t\t[ " +
        std::to_string( line ) + " ]" + "\t\t[" + std::string( file ) + "]";

    va_list args;
    va_start( args, format );
    vprintf( formattedString.c_str(), args );
    va_end( args );
    printf( "\n" );
}