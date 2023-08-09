#pragma once

#include <cstdint>

namespace Sk {
namespace Logging {

enum LogType {
    LogType_Debug,
    LogType_Info,
    LogType_Warning,
    LogType_Error
};

class Logger {
public:
    static void PrintOK();
    static void PrintFail();
    static void Log(LogType logtype, char *message);
};

} // namespace Dev
} // namespace Sk