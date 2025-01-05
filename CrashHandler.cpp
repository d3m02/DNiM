#include "CrashHandler.hpp"

#include <csignal>
#include <string>

#if __cplusplus >= 202002L
# include <format>
#endif 

#if __cplusplus < 201103L
# include <sstream>
# define nullptr NULL
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Logger.hpp"

namespace CrashHandler {

/** @brief Show simple crash dialog. Currently availible only on Windows */
void showReport(const std::string& cause)
{
#ifdef _WIN32
    MessageBox(nullptr, cause.c_str(), "Crashed!", MB_OK | MB_ICONERROR);
#endif
}

#if __cplusplus >= 202002L
constexpr
#endif
/** @brief Convert signal to human readable string */
std::string causeToString(int signal)
{
    switch (signal)
    {
    case SIGINT:  return "Interactive Attention (SIGINT)";
    case SIGILL:  return "Illegal Instruction (SIGILL): Invalid Function Image";
    case SIGFPE:  return "Arithmetic Exception (SIGFPE)";
    case SIGSEGV: return "Segmentation Fault (SIGSEGV)";
    case SIGTERM: return "Software Termination (SIGTERM)";
    case SIGABRT: return "Abort (SIGABRT)";
#ifdef _WIN32
    case SIGABRT_COMPAT: return "Abort (SIGABRT_COMPAT)";
#endif
    default:
    #if __cplusplus >= 202002L
        return std::format("Unknown signal ({})", signal);
    #elif __cplusplus >= 201103L    // C++11 at least
        return "Unknown signal (" + std::to_string(signal) + ")";
    #else   // C++98, seriously?
        std::ostringstream oss;
        oss << "Unknown signal (" << signal << ")";
        return oss.str();
    #endif
    }
}

#if __cplusplus >= 201103L
[[noreturn]]
#endif 
/** @brief Signal handler function */
void signalHandler(int signal)
{
    std::string cause = causeToString(signal);
    Log::Error(cause);
    showReport(cause);
    std::exit(signal);
}

/**  @brief Initialize the CrashHandler and register signal handler */
void init()
{
    signal(SIGINT, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGABRT, signalHandler);

#ifdef _WIN32
    signal(SIGABRT_COMPAT, signalHandler);
#endif
}

}   // namespace CrashHandler