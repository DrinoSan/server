#ifndef LOGGING_H
#define LOGGING_H

#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <string_view>

#define TRACE_INFO_LENGTH 7
#define TRACE_ERR_LENGTH  6

constexpr std::string_view InfoStr  = "[INFO]\t";
constexpr std::string_view ErrorStr = "[ERR]\t";

//-----------------------------------------------------------------------------
void traceInfo( const std::string format, ... );

//-----------------------------------------------------------------------------
void traceError( const std::string format, ... );

#endif   // LOGGING_H