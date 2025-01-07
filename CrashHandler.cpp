#include "CrashHandler.hpp"

#include <csignal>
#include <string>

#if __cplusplus >= 202002L
# include <format>
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

#include "Logger.hpp"

namespace CrashHandler {

/** @brief Show simple crash dialog. Currently availible only on Windows */
void showReport(std::string&& cause)
{
#ifdef _WIN32
    MessageBox(nullptr, std::move(cause).c_str(), "Crashed!", MB_OK | MB_ICONERROR);
#endif
}

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
    #else // C++11 fallback
        return std::string{"Unknown signal ("} + std::to_string(signal) + ")";
    #endif
    }
}

/** @brief Signal handler function */
[[noreturn]] void signalHandler(int signal)
{
    auto cause = causeToString(signal);
    Log::Error(cause);
    showReport(std::move(cause));
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