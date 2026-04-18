#include "UIPanel.h"

#include <algorithm>

#include "Tools/Logging.h"
#include "UI/Core/UIDeserializer.h"

namespace UI
{
    void Panel::AddChild(const ObjectSharedPtr& object)
    {
        if(object == nullptr)
        {
            Logging::LogWarning("Attempted to attach an empty UI Object child");
            return;
        }
        object->SetParent(*this);
        mChildren.emplace_back(object);
    }

    void Panel::RemoveChild(std::string_view childId)
    {
        const auto found = std::ranges::find_if(mChildren, [childId](const auto& child) {
            return child && child->Id() == childId;
        });

        if(found == std::end(mChildren))
        {
            Logging::LogWarning("Failed to find child object with id {} in object {}", childId, mPath);
            return;
        }

        mChildren.erase(found);
    }

    void Panel::RemoveChild(int childIndex)
    {
        if(childIndex >= mChildren.size())
        {
            Logging::LogWarning("Attempted to remove child with index {} beyond size of {} in object {}", childIndex,
                                mChildren.size(), mPath);
            return;
        }

        auto it = std::begin(mChildren);

        std::advance(it, childIndex);
        mChildren.erase(it);
    }

    void Panel::RemoveAllChildren() { mChildren.clear(); }

    void Panel::Update()
    {
        superclass::Update();
        for(const ObjectSharedPtr& child : mChildren)
        {
            child->Update();
        }
    }

    void Panel::Initialize(const pugi::xml_node& node)
    {
        superclass::Initialize(node);

        if(auto children = node.child("Children"))
        {
            for(auto it = children.begin(); it != children.end(); it++)
            {
                AddChild(DeserializeNode(*it));
            }
        }
    }

    void Panel::Draw() const
    {
        for(ObjectSharedPtr child : mChildren)
        {
            if(child)
            {
                child->Draw();
            }
        }
    }

    void Panel::DrawChildren() const
    {
        for(const ObjectSharedPtr& child : mChildren)
        {
            child->Draw();
        }
    }

    void Panel::UpdatePath()
    {
        superclass::UpdatePath();

        for(auto& children : mChildren)
        {
            if(children)
            {
                children->UpdatePath();
            }
        }
    }

    void Panel::DrawImguiChildrenObjects(const bool expand)
    {
        for(const ObjectSharedPtr& child : mChildren)
        {
            child->DrawImguiObjectTreeDebugMenu(expand);
        }
    }

    ObjectSharedPtr Panel::FindObjectByPath(std::string_view path)
    {
        if(ObjectSharedPtr found = superclass::FindObjectByPath(path))
        {
            return found;
        }

        static constexpr auto PATH_SEPARATOR = '.';

        // This can be more efficient by storing a path object
        // It's hella heavy to do all this string manipulation every time we search a path
        // Also we don't want to depend on '.'
        const auto frontSeparatorIndex = path.find_first_of(PATH_SEPARATOR);
        const auto frontId             = path.substr(0, frontSeparatorIndex);

        if(frontId != mId)
        {
            return nullptr;
        }

        const auto rest = path.substr(frontSeparatorIndex + 1);

        for(const ObjectSharedPtr& object : mChildren)
        {
            if(object)
            {
                if(ObjectSharedPtr foundObject = object->FindObjectByPath(rest))
                {
                    return foundObject;
                }
            }
        }

        return nullptr;
    }

    ObjectSharedPtr Panel::FindObjectById(std::string_view id)
    {
        if(ObjectSharedPtr found = superclass::FindObjectById(id))
        {
            return found;
        }

        for(const ObjectSharedPtr& object : mChildren)
        {
            if(object)
            {
                if(ObjectSharedPtr foundObject = object->FindObjectById(id))
                {
                    return foundObject;
                }
            }
        }

        return nullptr;
    }

    int Panel::GetImguiObjectTreeDebugNodeData(const bool forceExpand)
    {
        if(mChildren.empty())
        {
            return ImGuiTreeNodeFlags_Leaf;
        }
        else if(forceExpand)
        {
            return ImGuiTreeNodeFlags_DefaultOpen;
        }

        return superclass::GetImguiObjectTreeDebugNodeData(forceExpand);
    }
} // namespace UI