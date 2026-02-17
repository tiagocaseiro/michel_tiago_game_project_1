#pragma once

#include <chrono>
#include <iostream>
#include <print>
#include <string_view>

namespace Logging
{
    enum class LoggingLevel
    {
        Info    = 1 << 0,
        Debug   = 1 << 1,
        Warning = 1 << 2,
        Error   = 1 << 3,
        All     = -1
    };

    template <LoggingLevel level>
    void Log(std::string_view message);

    static auto LogInfo    = Log<LoggingLevel::Info>;
    static auto LogDebug   = Log<LoggingLevel::Debug>;
    static auto LogWarning = Log<LoggingLevel::Warning>;
    static auto LogError   = Log<LoggingLevel::Error>;

    template <LoggingLevel level>
    void EnableLevel();

    static auto EnableInfoLevel    = EnableLevel<LoggingLevel::Info>;
    static auto EnableDebugLevel   = EnableLevel<LoggingLevel::Debug>;
    static auto EnableWarningLevel = EnableLevel<LoggingLevel::Warning>;
    static auto EnableErrorLevel   = EnableLevel<LoggingLevel::Error>;
    static auto EnableAllLevels    = EnableLevel<LoggingLevel::All>;

    template <LoggingLevel level>
    void DisableLevel();

    static auto DisableInfoLevel    = DisableLevel<LoggingLevel::Info>;
    static auto DisableDebugLevel   = DisableLevel<LoggingLevel::Debug>;
    static auto DisableWarningLevel = DisableLevel<LoggingLevel::Warning>;
    static auto DisableErrorLevel   = DisableLevel<LoggingLevel::Error>;
    static auto DisableAllLevels    = DisableLevel<LoggingLevel::All>;

    void PrintDebugMenu();

} // namespace Logging
