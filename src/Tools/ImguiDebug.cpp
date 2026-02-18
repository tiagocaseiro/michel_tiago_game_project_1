#include "ImguiDebug.h"

#include "Logging.h"

#include "imgui.h"

static bool show_log_window = false;

namespace ImguiDebug
{
    static void DrawMainMenuBar()
    {
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Debug"))
            {
                if(ImGui::MenuItem("Log"))
                {
                    show_log_window = !show_log_window;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void Init() {}

    void DrawMenus()
    {
        DrawMainMenuBar();

        if(show_log_window)
        {
            ImGui::Begin("Log", &show_log_window);
            Logging::PrintDebugMenu();
            ImGui::End();
        }
    }

    void Destroy() {}

} // namespace ImguiDebug