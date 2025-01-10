#pragma once

#if __cplusplus >= 201703L
# include <string_view>
#endif

#if __cplusplus >= 202002L
# include <source_location>
#else
# include <string>
#endif


/**
 * @brief Log functions implementation for Dignostics and Information System (DnIS)  
 */
namespace Log {

enum class LogLevel : char
{
    UnkownLevel = 0,
    DebugLevel,
    InfoLevel,
    WarringLevel,
    ErrorLevel
};

void test(Log::LogLevel level, std::string_view, std::source_location);

#if __cplusplus >= 202002L

/** @brief log-level message handler function */
void LogHandler(Log::LogLevel, std::string_view, std::source_location);


/** @brief Print message with [DEBUG] label and mesasge source */
inline void Debug(std::string_view message, std::source_location source = std::source_location::current()) 
{ Log::LogHandler(Log::LogLevel::DebugLevel, message, source); }

/** @brief Print message with [INFO] label and mesasge source */
inline void Info(std::string_view message, std::source_location source = std::source_location::current()) 
{ Log::LogHandler(Log::LogLevel::InfoLevel, message, source); }

/** @brief Print message with [WARNING] label and mesasge source */
inline void Warning(std::string_view message, std::source_location source = std::source_location::current()) 
{ Log::LogHandler(Log::LogLevel::WarringLevel, message, source); }

/** @brief Print message with [ERROR] label and mesasge source */
inline void Error(std::string_view message, std::source_location source = std::source_location::current()) 
{ Log::LogHandler(Log::LogLevel::ErrorLevel, message, source); }

#else // Compitabily fallback version, designed to support C++11 and later
    class LogObject
    {
    public:
        explicit LogObject(const char* file, const char* function, const unsigned int line, const Log::LogLevel level)
            : m_file(file), m_function(function), m_line(line), m_level(level)
        {}

#if __cplusplus >= 201703L
        LogObject& operator() (std::string_view message);
#else 
        LogObject& operator() (const std::string& message);
#endif
    
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
    #define Debug LogObject(__FILE__, __func__, __LINE__, Log::LogLevel::DebugLevel)
    /** @brief Print message with [INFO] label and mesasge source */
    #define Info LogObject(__FILE__, __func__, __LINE__, Log::LogLevel::InfoLevel)
    /** @brief Print message with [WARNING] label and mesasge source */
    #define Warning LogObject(__FILE__, __func__, __LINE__, Log::LogLevel::WarringLevel)
    /** @brief Print message with [ERROR] label and mesasge source */
    #define Error LogObject(__FILE__, __func__, __LINE__, Log::LogLevel::ErrorLevel)

#endif
}