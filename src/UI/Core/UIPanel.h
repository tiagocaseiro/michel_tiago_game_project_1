#pragma once

#include "UIObject.h"

namespace UI
{
    class Panel : public Object
    {
        DECLARE_UI_ELEMENT(Panel);

    public:
        virtual ~Panel() = default;

        void AddChild(const ObjectSharedPtr& object);
        void RemoveChild(std::string_view childId);
        void RemoveChild(int childIndex);
        void RemoveAllChildren();

        const std::vector<std::shared_ptr<Object>>& Children() const { return mChildren; }

        ObjectSharedPtr FindObjectById(std::string_view id) override;

        template <typename T>
        std::shared_ptr<T> FindObjectById(std::string_view id)
        {
            return std::dynamic_pointer_cast<T>(FindObjectById(id));
        }

    protected:
        void Update() override;

        void Initialize(const pugi::xml_node& node);

        void Draw() const override;
        void DrawChildren() const;
        void UpdatePath() override;
        void DrawImguiChildrenObjects(const bool expand) override;

        ObjectSharedPtr FindObjectByPath(std::string_view path) override;

        int GetImguiObjectTreeDebugNodeData(const bool forceExpand) override;

        std::vector<ObjectSharedPtr> mChildren;
    };
} // namespace UI
