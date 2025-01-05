#include "Logger.hpp"

#include <iostream>

#if __cplusplus >= 202002L
# include <format>
# include <string_view>
#elif __cplusplus < 201103L
# include <sstream>
#endif

namespace Log {

/** Hidding from user of header internal implementation */
#if __cplusplus >= 202002L
    using enum Log::LogLevel;
#endif

#if __cplusplus >= 202002L
constexpr std::string_view
#else
const char* 
#endif
/** @brief Printable log level label + decoration */
logLevelStr(Log::LogLevel level)
{
    switch (level) 
    {
        case DebugLevel:    return "[DEBUG]\t";
        case InfoLevel:     return "[INFO]\t";
        case WarringLevel:  return "[WARNING]\t";
        case ErrorLevel:    return "[ERROR]\t";
        default:            return "[UNKNOWN]\t";
    }
}

#if __cplusplus >= 202002L
/** @brief Format log message and output in cout */
void printMsg(LogLevel level, std::string_view source, std::string_view message)
#else
/** @brief Format log message and output in cout */
void printMsg(LogLevel level, const std::string& source, const std::string& message)
#endif
{
    std::cout << logLevelStr(level) 
              << "| "<< source << " \t| "
              << message << "\n";
}

#if __cplusplus >= 202302L
constexpr
#endif
#if __cplusplus >= 202002L
/** @brief Format and decorate log message source */
std::string getPrittyName(const std::source_location& source)
{
    return std::format("{}:{} {}", source.file_name(),
                                   source.line(),
                                   source.function_name());
};
#else
/** @brief Format and decorate log message source */
std::string LogObject::getPrittyName()
{
// TODO: constructing std::string via concatenation migbe lees efficient than c++98 version, compare and remove if redundant
# if __cplusplus >= 201103L
    return std::string(file_name()) + ":" + std::to_string(line()) + " " + function_name();
# else 
    std::ostringstream oss;
    oss << file_name() << ":" << line() << " " << function_name();
    return oss.str();
# endif
};
#endif

/** C++20 implementation using std::source_location for decorrating log message */
#if __cplusplus >= 202002L

// TODO: Compare template approach with obect-oriented approach and select better option
template<LogLevel level>
void LogHandler(std::string_view message, std::source_location source)
{
    printMsg(level, getPrittyName(source), message);
}

/** Explicit instantiation of templates */
template void LogHandler<DebugLevel>(std::string_view, std::source_location);
template void LogHandler<InfoLevel>(std::string_view, std::source_location);
template void LogHandler<WarringLevel>(std::string_view, std::source_location);
template void LogHandler<ErrorLevel>(std::string_view, std::source_location);

#else // Compitabily fallback version, designed to support C++98 and later

/** @brief trick user with Log::Error() - it's not an function call, it's create LogObject(...).operator(..) */
LogObject& LogObject::operator() (const std::string& message)
{
    printMsg(this->m_level, getPrittyName(), message);
    return *this;
}

#endif

} // namespace Log