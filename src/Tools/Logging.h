#pragma once

#include <chrono>
#include <iostream>
#include <string_view>

#include "imgui.h"

class Logging
{
    enum class LoggingLevel
    {
        Info    = 1 << 0,
        Debug   = 1 << 1,
        Warning = 1 << 2,
        Error   = 1 << 3,
        All     = -1
    };

public:
    template <typename... Args>
    static void LogInfo(std::string_view message, Args&&... args)
    {
        Log<LoggingLevel::Info>(message, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void LogDebug(std::string_view message, Args&&... args)
    {
        Log<LoggingLevel::Debug>(message, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void LogWarning(std::string_view message, Args&&... args)
    {
        Log<LoggingLevel::Warning>(message, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void LogError(std::string_view message, Args&&... args)
    {
        Log<LoggingLevel::Error>(message, std::forward<Args>(args)...);
    }

    template <LoggingLevel level>
    static void EnableLevel()
    {
        activeLevels |= (int)level;
    }

    static void EnableInfoLevel() { EnableLevel<LoggingLevel::Info>(); }
    static void EnableDebugLevel() { EnableLevel<LoggingLevel::Debug>(); }
    static void EnableWarningLevel() { EnableLevel<LoggingLevel::Warning>(); }
    static void EnableErrorLevel() { EnableLevel<LoggingLevel::Error>(); }
    static void EnableAllLevels() { EnableLevel<LoggingLevel::All>(); }

    template <LoggingLevel level>
    static void DisableLevel()
    {
        activeLevels &= ~(int)level;
    }

    static void DisableInfoLevel() { DisableLevel<LoggingLevel::Info>(); }
    static void DisableDebugLevel() { DisableLevel<LoggingLevel::Debug>(); }
    static void DisableWarningLevel() { DisableLevel<LoggingLevel::Warning>(); }
    static void DisableErrorLevel() { DisableLevel<LoggingLevel::Error>(); }
    static void DisableAllLevels() { DisableLevel<LoggingLevel::All>(); }

    static void DrawImguiDebugMenu();

private:
    template <LoggingLevel level>
    static bool IsLevelActive()
    {
        const bool isActive = activeLevels & (int)level;
        return isActive != 0;
    }

    template <LoggingLevel level, typename... Args>
    static void Log(std::string_view message, Args&&... args)
    {
        if(IsLevelActive<level>() == false)
        {
            return;
        }

        const auto now  = std::chrono::system_clock::now();
        const auto diff = std::chrono::hh_mm_ss(now - begin);

        std::cout << diff;
        switch(level)
        {
            case LoggingLevel::Info:
                std::cout << "\033[34m [Info] \033[0m";
                break;
            case LoggingLevel::Debug:
                std::cout << "\033[32m [Debug] \033[0m";
                break;
            case LoggingLevel::Warning:
                std::cout << "\033[33m [Warning] \033[0m";
                break;
            case LoggingLevel::Error:
                std::cout << "\033[31m [Error] \033[0m";
                break;
        }

        std::cout << std::vformat(message, std::make_format_args(args...)) << std::endl;
    }

    template <LoggingLevel level>
    static void DrawLevelImguiDebugMenu()
    {
        ImGui::PushID((int)level);
        switch(level)
        {
            case LoggingLevel::Info:
                ImGui::Text("Info");
                break;
            case LoggingLevel::Debug:
                ImGui::Text("Debug");
                break;
            case LoggingLevel::Warning:
                ImGui::Text("Warning");
                break;
            case LoggingLevel::Error:
                ImGui::Text("Error");
                break;
        }

        ImGui::SameLine();
        const bool isActive = IsLevelActive<level>();
        if(ImGui::Button(isActive ? "Disable" : "Enable"))
        {
            if(isActive)
            {
                DisableLevel<level>();
            }
            else
            {
                EnableLevel<level>();
            }
        }
        ImGui::PopID();
    }

    static std::chrono::system_clock::time_point begin;
    static int activeLevels;
};
