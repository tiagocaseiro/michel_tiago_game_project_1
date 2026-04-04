#pragma once

#include "UI/Core/UIObject.h"

namespace UI
{
    namespace Store
    {
        ObjectSharedPtr Clone(std::string_view filePath);
        void LoadTemplates();
    } // namespace Store

} // namespace UI
