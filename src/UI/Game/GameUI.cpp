#include "UI/Game/GameUI.h"

#include "UI/Core/UIObject.h"

GameUI& GameUI::Instance()
{
    static GameUI gameUI;
    return gameUI;
}

void GameUI::InitializeStartUpUI()
{
    UI::RemoveAllObjects();

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

    UI::AddObject(std::move(image));
}

void GameUI::OnMouseLeftButtonUp(const float mouseX, const float mouseY)
{
    if(UI::Object* object = UI::FindObjectByPath("image.panel1.panel11"))
    {
        UI::RemoveAllObjects();
    }
}
