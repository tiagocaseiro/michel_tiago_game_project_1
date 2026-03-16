#include "ImguiDebug.h"

#include "imgui.h"

#include "Model/GameManager.h"
#include "Tools/Logging.h"
#include "UI/Core/UIObject.h"

static bool show_log_window   = false;
static bool show_ui_window    = true;
static bool show_model_window = true;

namespace ImguiDebug
{
    static void DrawMainMenuBar()
    {
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::MenuItem("Log"))
            {
                show_log_window = !show_log_window;
            }

            if(ImGui::MenuItem("UI"))
            {
                show_ui_window = !show_ui_window;
            }

            if(ImGui::MenuItem("Model"))
            {
                show_model_window = !show_model_window;
            }

            ImGui::EndMainMenuBar();
        }
    }

    void Init() {}

    void DrawMenus(const GameManager& game)
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
            ImGui::Begin("UI Object Tree", &show_ui_window);
            UI::DrawImguiObjectTreeDebugMenu(true);
            ImGui::End();
        }

        if(show_model_window)
        {
            ImGui::Begin("Model", &show_model_window);
            static char buf1[128]            = "saveFileName";
            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AlwaysOverwrite;
            ImGui::InputText("Save file name:", buf1, IM_COUNTOF(buf1), flags);
            if(ImGui::Button("Save"))
            {
                game.Save(buf1);
            }
            ImGui::End();
        }
    }

    void Destroy() {}

} // namespace ImguiDebug