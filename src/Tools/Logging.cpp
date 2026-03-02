#include "Logging.h"

std::chrono::system_clock::time_point Logging::begin = std::chrono::system_clock::now();

int Logging::activeLevels = (int)LoggingLevel::All;

void Logging::DrawImguiDebugMenu()
{
    DrawLevelImguiDebugMenu<LoggingLevel::Info>();
    DrawLevelImguiDebugMenu<LoggingLevel::Debug>();
    DrawLevelImguiDebugMenu<LoggingLevel::Warning>();
    DrawLevelImguiDebugMenu<LoggingLevel::Error>();

    if(ImGui::Button("Enable All"))
    {
        EnableAllLevels();
    }

    if(ImGui::Button("Disable All"))
    {
        DisableAllLevels();
    }
}
