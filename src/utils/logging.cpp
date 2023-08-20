#include "logging.h"
#include <iostream>
#include <string>

// ANSI escape codes for text colors
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"

//-----------------------------------------------------------------------------
void LogInfo(int line, const char* file, const std::string format, ...)
{
    const std::string formattedString =
        std::string(ANSI_COLOR_GREEN) + "[INFO] " + format + "\t\t[ " +
        std::to_string(line) + " ]" + "\t\t[" + std::string(file) + "]" +
        ANSI_COLOR_RESET;

    va_list args;
    va_start(args, format);
    vprintf(formattedString.c_str(), args);
    va_end(args);
    printf("\n");
}

//-----------------------------------------------------------------------------
void LogError(int line, const char* file, const std::string format, ...)
{
    const std::string formattedString =
        std::string(ANSI_COLOR_RED) + "[ERROR] " + format + "\t\t[ " +
        std::to_string(line) + " ]" + "\t\t[" + std::string(file) + "]" +
        ANSI_COLOR_RESET;

    va_list args;
    va_start(args, format);
    vprintf(formattedString.c_str(), args);
    va_end(args);
    printf("\n");
}
