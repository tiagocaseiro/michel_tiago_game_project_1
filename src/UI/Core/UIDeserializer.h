#pragma once

#include <memory>
#include <string>

namespace pugi
{
    class xml_node;
}

namespace UI
{
    class Object;

    std::shared_ptr<UI::Object> DeserializeLayout(std::string_view filePath);
    std::shared_ptr<Object> DeserializeNode(const pugi::xml_node& node);

} // namespace UI
