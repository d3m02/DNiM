#include "Logger.hpp"

#include <iostream>

#if __cplusplus >= 202002L
# include <format>
#endif

namespace Log {

/** @brief Printable log level label + decoration */
const char* logLevelStr(Log::LogLevel level)
{
    switch (level) 
    {
        case Log::LogLevel::DebugLevel:    return "[DEBUG]\t";
        case Log::LogLevel::InfoLevel:     return "[INFO]\t";
        case Log::LogLevel::WarringLevel:  return "[WARNING]\t";
        case Log::LogLevel::ErrorLevel:    return "[ERROR]\t";
        default:                           return "[UNKNOWN]\t";
    }
}


#if __cplusplus >= 201703L
/** @brief Format log message and output in cout */
void printMsg(const char* level, std::string_view source, std::string_view  message)
#else
/** @brief Format log message and output in cout */
void printMsg(const char* level, std::string&& source, const std::string& message)
#endif
{
    std::cout << level << message 
              << " @ " << source << "\n";
}


#if __cplusplus >= 202002L
    
/** @brief Format and decorate log message source */
std::string getPrittyName(const std::source_location& source)
{
    return std::format("{}:{} `{}`", source.file_name(),
                                     source.line(),
                                     source.function_name());
};

#else

/** @brief Format and decorate log message source */
std::string LogObject::getPrittyName()
{
    return std::string(file_name()) + ":" + std::to_string(line()) + " `" + function_name() + '`';
};

#endif

/* C++20 implementation can take advantages of std::source_location for decorrating log message */
#if __cplusplus >= 202002L

using enum Log::LogLevel;
// TODO: Compare overload approach with obect-oriented approach and select better option
void LogHandler(Log::LogLevel level, std::string_view message, std::source_location source)
{
    printMsg(logLevelStr(level), getPrittyName(source), message);
}

#else  

// trick user with Log::Error() - it's not an function call, it's create LogObject(...).operator(..)
#if __cplusplus >= 201703L
LogObject& LogObject::operator() (std::string_view message)
#else
LogObject& LogObject::operator() (const std::string& message)
#endif
{
    printMsg(logLevelStr(this->m_level), getPrittyName(), message);
    return *this;
}

#endif

} // namespace Log