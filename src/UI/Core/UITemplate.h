#pragma once

#include <memory>

namespace pugi
{
    class xml_node;
}

namespace UI
{
    class Object;

    class Template
    {
    public:
        static std::shared_ptr<Object> Make(const pugi::xml_node& node);
    };
} // namespace UI