#pragma once

#include <memory>
#include <string>
#include <type_traits>
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
    template <typename T = Object>
    std::shared_ptr<T> DeserializeObject(const std::string& filePath)
    {
        std::vector<std::shared_ptr<UI::Object>> loadedTemplateObjects = UI::DeserializeLayout(filePath);
        if(loadedTemplateObjects.empty())
        {
            return {};
        }

        if constexpr(std::is_same_v<T, Object>)
        {
            return loadedTemplateObjects.front();
        }
        else
        {
            return std::dynamic_pointer_cast<T>(loadedTemplateObjects.front());
        }
        return {};
    }

} // namespace UI
