#pragma once

#include <iostream>

#include <pugixml.hpp>

#include "Tools/Logging.h"
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

    std::vector<std::shared_ptr<Object>> DeserializeLayout(const std::string& filePath)
    {
        static const std::string s_uiFolder = "assets/ui/";

        const std::string finalPath = s_uiFolder + filePath;

        std::vector<std::shared_ptr<Object>> rootObjects;

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(finalPath.data());
        if(result == false)
        {
            Logging::LogWarning("Failed to load layout file {}: {}", finalPath, result.description());
            return rootObjects;
        }

        for(auto it = doc.begin(); it != doc.end(); it++)
        {
            rootObjects.push_back(DeserializeNode(*it));
        }

        return rootObjects;
    }
} // namespace UI
