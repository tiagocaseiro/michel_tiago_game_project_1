#include "UITemplate.h"

#include <pugixml.hpp>

#include "UI/Core/UIDeserializer.h"

namespace UI
{
    std::shared_ptr<Object> Template::Make(const pugi::xml_node& node)
    {
        if(std::string("Template") == node.name())
        {
            if(auto path = node.child("Path"))
            {
                return DeserializeObject(path.text().as_string());
            }
        }
        return {};
    }
} // namespace UI