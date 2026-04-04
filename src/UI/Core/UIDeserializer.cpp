#pragma once

#include <iostream>

#include <pugixml.hpp>

#include "UI/Core/UIDeserializer.h"
#include "UI/Core/UIMaterial.h"
#include "UI/Core/UIObject.h"
#include "UI/Core/UIStackPanel.h"
#include "UI/Core/UIText.h"

namespace UI
{

    std::shared_ptr<Object> DeserializeNode(const pugi::xml_node& node)
    {
        if(auto object = StackPanel::Make(node))
        {
            return object;
        }

        if(auto object = Material::Make(node))
        {
            return object;
        }

        if(auto object = Text::Make(node))
        {
            return object;
        }

        return {};
    }

    std::shared_ptr<Object> DeserializeLayout(std::string_view filePath)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(filePath.data());
        if(result == false)
        {
            return {};
        }
        auto it = doc.begin();

        return DeserializeNode(*it);
    }
} // namespace UI
