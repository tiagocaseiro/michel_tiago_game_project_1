#pragma once

#include "UIDeserializer.h"
#include "UIStore.h"

namespace UI
{
    namespace Store
    {
        void LoadTemplates() { DeserializeLayout("button.ui_layout"); }
        ObjectSharedPtr Clone(std::string_view filePath) { return ObjectSharedPtr(); }
    } // namespace Store
} // namespace UI
