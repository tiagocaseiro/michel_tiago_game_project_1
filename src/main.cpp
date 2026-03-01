#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "Common/Data.h"
#include "Common/Fonts.h"

#include "Tools/ImguiDebug.h"
#include "Tools/Logging.h"

#include "Model/Card.h"
#include "Model/CardColour.h"
#include "Model/CardType.h"
#include "Model/CardZone.h"

#include "UI/UIObject.h"

int main(int, char**)
{
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        Logging::LogError("SDL failed to initialize: {}", SDL_GetError());
        return 1;
    }

    // Create window with SDL_Renderer graphics context
    float main_scale             = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window           = SDL_CreateWindow("Dear ImGui SDL3+SDL_Renderer example", (int)(1280 * main_scale),
                                                    (int)(800 * main_scale), window_flags);
    if(window == nullptr)
    {
        Logging::LogError("Window failed to initialize: {}", SDL_GetError());
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    Common::SetRenderer(renderer);

    SDL_SetRenderVSync(renderer, 1);
    if(renderer == nullptr)
    {
        Logging::LogError("Renderer failed to initialize: {}", SDL_GetError());
        return 1;
    }

    if(TTF_Init() == false)
    {
        Logging::LogError("SDL_ttf failed to initialize: {}", SDL_GetError());
        return 1;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling,
                                     // changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale; // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true
                                     // automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Common::LoadFont("fonts/unispace.ttf", 200);

    auto text = UI::Text::Make("text");
    text->SetFontPath("fonts/unispace.ttf");
    text->SetWidth(300);
    text->SetHeight(30);
    text->SetColor({0, 0, 0, 1});
    text->SetMargin({100, 50, 0, 0});
    text->SetText("Pray for Israel <3");

    auto panel1 = UI::Material::Make("panel1");
    panel1->SetWidth(100);
    panel1->SetHeight(100);
    panel1->SetMargin({100, 100, 0, 0});
    panel1->SetColor({0.5, 0.26, 0.75, 1});

    auto panel11 = UI::Material::Make("panel11");
    panel11->SetWidth(25);
    panel11->SetHeight(25);
    panel11->SetMargin({10, 10, 0, 0});
    panel11->SetColor({1, 1, 0.75, 1});

    panel1->AddChild(std::move(panel11));

    auto panel2 = UI::Material::Make("panel2");
    panel2->SetWidth(100);
    panel2->SetHeight(100);
    panel2->SetMargin({200, 200, 0, 0});
    panel2->SetColor({1, 0.76, 0, 1});

    auto panel3 = UI::Material::Make("panel3");
    panel3->SetWidth(100);
    panel3->SetHeight(100);
    panel3->SetVisibility(false);
    panel3->SetMargin({300, 300, 0, 0});
    panel3->SetColor({0.5, 1., 0.2, 0.4});

    auto image = UI::Material::Make("image");
    image->SetWidth(500);
    image->SetHeight(500);
    image->SetMargin({300, 300, 0, 0});
    image->SetImagePath("textures/crate.png");
    image->SetColor({1, 0, 0, 0.40});

    image->AddChild(std::move(panel1));
    image->AddChild(std::move(panel2));
    image->AddChild(std::move(panel3));
    image->AddChild(std::move(text));

    UI::Root().AddChild(std::move(image));

    // Main loop
    bool done = false;
    while(!done)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if(event.type == SDL_EVENT_QUIT)
                done = true;
            if(event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        if(SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        UI::Update();

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImguiDebug::DrawMenus();

        // Rendering
        ImGui::Render();
        SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        UI::Draw();
        UI::DrawDebug();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    Common::UnloadAllFonts();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}