#include "ImguiDebug.h"

#include "imgui.h"

#include "Logging.h"
#include "UI/UIObject.h"

static bool show_log_window = false;
static bool show_ui_window  = false;

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

            if(ImGui::MenuItem("UI"))
            {
                show_ui_window = !show_ui_window;
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
            Logging::DrawImguiDebugMenu();
            ImGui::End();
        }

        if(show_ui_window)
        {
            UI::Root().DrawImguiObjectTreeDebugMenu();
        }
    }

    void Destroy() {}

} // namespace ImguiDebug