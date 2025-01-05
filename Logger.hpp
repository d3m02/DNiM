#pragma once

#if __cplusplus >= 202002L
# include <source_location>
# include <string_view>
#endif

#include <string>

/**
 * @brief Log functions implementation for Dignostics and Information System (DnIS)  
 */
namespace Log {
#if __cplusplus >= 202002L
    enum class LogLevel : char
#else
    enum LogLevel
#endif
    {
        UnkownLevel = 0,
        DebugLevel,
        InfoLevel,
        WarringLevel,
        ErrorLevel
    };

#if __cplusplus >= 202002L

// Template handler declaration
template<Log::LogLevel level>
void LogHandler(std::string_view message, std::source_location);

/** @brief Print message with [DEBUG] label and mesasge source */
inline void Debug(std::string_view message, std::source_location source = std::source_location::current()) 
{ LogHandler<Log::LogLevel::DebugLevel>(message, source); }

/** @brief Print message with [INFO] label and mesasge source */
inline void Info(std::string_view message, std::source_location source = std::source_location::current()) 
{ LogHandler<Log::LogLevel::InfoLevel>(message, source); }

/** @brief Print message with [WARNING] label and mesasge source */
inline void Warning(std::string_view message, std::source_location source = std::source_location::current()) 
{ LogHandler<Log::LogLevel::WarringLevel>(message, source); }

/** @brief Print message with [ERROR] label and mesasge source */
inline void Error(std::string_view message, std::source_location source = std::source_location::current()) 
{ LogHandler<Log::LogLevel::ErrorLevel>(message, source); }

#else // Compitabily fallback version, designed to support C++98 and later
    class LogObject
    {
    public:
        explicit LogObject(const char* file, const char* function, const unsigned int line, const Log::LogLevel level)
            : m_file(file), m_function(function), m_line(line), m_level(level)
        {}

        LogObject& operator() (const std::string& message);
    
    private:
        // Mimicate std::source_location member functions
        unsigned int line() const { return m_line; }
        const char* file_name() const { return m_file; }
        const char* function_name() const { return m_function; }

        std::string getPrittyName();
        
        const char* m_file;
        const char* m_function;
        unsigned int m_line;
        Log::LogLevel m_level;
    };

    /** @brief Print message with [DEBUG] label and mesasge source */
    #define Debug LogObject(__FILE__, __func__, __LINE__, Log::DebugLevel)
    /** @brief Print message with [INFO] label and mesasge source */
    #define Info LogObject(__FILE__, __func__, __LINE__, Log::InfoLevel)
    /** @brief Print message with [WARNING] label and mesasge source */
    #define Warning LogObject(__FILE__, __func__, __LINE__, Log::WarringLevel)
    /** @brief Print message with [ERROR] label and mesasge source */
    #define Error LogObject(__FILE__, __func__, __LINE__, Log::ErrorLevel)

#endif
}