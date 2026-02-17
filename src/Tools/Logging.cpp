#include "Logging.h"

#include <chrono>
#include <iostream>
#include <print>
#include <type_traits>

#include "imgui.h"

namespace Logging
{
    static const auto begin = std::chrono::system_clock::now();

    static int activeLevels =
        (int)LoggingLevel::Info | (int)LoggingLevel::Debug | (int)LoggingLevel::Warning | (int)LoggingLevel::Error;

    template <LoggingLevel level>
    bool IsLevelActive()
    {
        const bool isActive = activeLevels & (int)level;
        return isActive != 0;
    }

    template <LoggingLevel level>
    void Log(std::string_view message)
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

        std::cout << message << std::endl;
    }

    template <LoggingLevel level>
    void EnableLevel()
    {
        activeLevels |= (int)level;
    }

    template <LoggingLevel level>
    void DisableLevel()
    {
        activeLevels &= ~(int)level;
    }

    template <LoggingLevel level>
    void PrintDebugButton()
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

    void PrintDebugMenu()
    {
        PrintDebugButton<LoggingLevel::Info>();
        PrintDebugButton<LoggingLevel::Debug>();
        PrintDebugButton<LoggingLevel::Warning>();
        PrintDebugButton<LoggingLevel::Error>();

        if(ImGui::Button("Enable All"))
        {
            EnableAllLevels();
        }

        if(ImGui::Button("Disable All"))
        {
            DisableAllLevels();
        }
    }
} // namespace Logging