#ifndef LOGGING_H
#define LOGGING_H

#include <iomanip>
#include <ostream>
#include <stdarg.h>
#include <string>
#include <string_view>

#define TRACE_INFO_LENGTH 7
#define TRACE_ERR_LENGTH  6

constexpr std::string_view InfoStr  = "[INFO]\t";
constexpr std::string_view ErrorStr = "[ERR]\t";

#define traceInfo( format, ... )                                               \
    LogInfo( __LINE__, __FILE__, format, __VA_ARGS__ )
#define traceWarning( format, ... )                                            \
    LogWarning( __LINE__, __FILE__, format, __VA_ARGS__ )
#define traceError( format, ... )                                              \
    LogError( __LINE__, __FILE__, format, __VA_ARGS__ )

//-----------------------------------------------------------------------------
void LogInfo( int line, const char* file, const std::string format, ... );

//-----------------------------------------------------------------------------
void LogError( int line, const char* file, const std::string format, ... );

#endif   // LOGGING_H