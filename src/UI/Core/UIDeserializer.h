#pragma once

#include <memory>
#include <string>
#include <vector>

namespace pugi
{
    class xml_node;
}

namespace UI
{
    class Object;

    std::vector<std::shared_ptr<Object>> DeserializeLayout(const std::string& filePath);
    std::shared_ptr<UI::Object> DeserializeNode(const pugi::xml_node& node);

} // namespace UI
