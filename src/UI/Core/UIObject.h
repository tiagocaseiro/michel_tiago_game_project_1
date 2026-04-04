#pragma once

#include <string>
#include <vector>

#include <pugixml.hpp>

#include "Common/Color.h"
#include "Common/Text.h"
#include "Common/Texture.h"

#include "Tools/ImguiDebug.h"

#define DECLARE_UI_ELEMENT_DERIVED(UIClassName, UIClassNameParent)                                                     \
public:                                                                                                                \
    using superclass = UIClassNameParent;                                                                              \
    explicit UIClassName(const std::string& id) : UIClassNameParent(id) {}                                             \
    static auto Make(const std::string& id) { return std::shared_ptr<UIClassName>(new UIClassName(id)); }              \
    static auto Make(const pugi::xml_node& node)                                                                       \
    {                                                                                                                  \
        if(node.name() != ClassId())                                                                                   \
        {                                                                                                              \
            return std::shared_ptr<UIClassName>{};                                                                     \
        }                                                                                                              \
        const std::string id = node.child("Id").text().as_string();                                                    \
        if(id.empty())                                                                                                 \
        {                                                                                                              \
            return std::shared_ptr<UIClassName>{};                                                                     \
        }                                                                                                              \
        auto object = Make(id);                                                                                        \
        object->Initialize(node);                                                                                      \
        return object;                                                                                                 \
    }                                                                                                                  \
    static std::string ClassId() { return #UIClassName; }

#define DECLARE_UI_ELEMENT(UIClassName) DECLARE_UI_ELEMENT_DERIVED(UIClassName, Object)

namespace pugi
{
    class xml_node;
}

namespace UI
{
    using ObjectSharedPtr = std::shared_ptr<class Object>;
    using ObjectWeakPtr   = std::weak_ptr<class Object>;

    enum class HorizontalAlignment
    {
        Left,
        Right,
        Center,
        Stretch
    };

    enum class VerticalAlignment
    {
        Top,
        Bottom,
        Center,
        Stretch
    };

    struct Margin
    {
        float left   = 0.0f;
        float top    = 0.0f;
        float right  = 0.0f;
        float bottom = 0.0f;
    };

    using PositionDimension = SDL_FRect;
    using Color             = SDL_FColor;

    void Update();
    void Draw();
    void DrawDebug();

    class Object
    {
        friend class StackPanel;

    public:
        Object(const Object&)            = delete;
        Object& operator=(const Object&) = delete;
        virtual ~Object();

        virtual void Draw() const = 0;

        void SetHeight(float const height) { mPositionDimension.h = height; }
        void SetWidth(float const width) { mPositionDimension.w = width; }
        void SetMargin(Margin const margin) { mMargin = margin; }
        void SetTopMargin(float const topMargin) { mMargin.top = topMargin; }
        void SetBottomMargin(float const bottomMargin) { mMargin.bottom = bottomMargin; }
        void SetLeftMargin(float const leftMargin) { mMargin.left = leftMargin; }
        void SetRightMargin(float const rightMargin) { mMargin.right = rightMargin; }
        void SetVisibility(bool const visibility) { mVisible = visibility; }
        void SetHorizontalAlignment(HorizontalAlignment const hAlignment) { mHorizontalAlignment = hAlignment; }
        void SetVerticalAlignment(VerticalAlignment const vAlignment) { mVerticalAlignment = vAlignment; }
        void SetParent(Object& parent);

        const std::vector<std::shared_ptr<Object>>& Children() { return mChildren; }

        bool IsInsideBounds(const float x, const float y);

        virtual void Update();

        virtual void DrawImguiObjectTreeDebugMenu(const bool forceExpand);
        virtual void DrawImguiObjectDetailsDebugMenu();

        void AddChild(const ObjectSharedPtr& object);

        Object* FindObjectByPath(std::string_view path);

    protected:
        bool GetVisibility() const { return mVisible; }

        void UpdatePath();
        void RemoveChild(std::string_view childId);
        void RemoveChild(int childIndex);
        void RemoveAllChildren();

        void UpdateDimensions();
        void UpdatePosition();

        void DrawChildren() const;

        void Initialize(const pugi::xml_node& node);

        explicit Object(const std::string& id)
            : mId(id),
              mParent(nullptr),
              mPath(id),
              mPositionDimension{0.0f, 0.0f, 0.0f, 0.0f},
              mMargin{0.0f, 0.0f, 0.0f, 0.0f},
              mHorizontalAlignment{HorizontalAlignment::Left},
              mVerticalAlignment{VerticalAlignment::Top},
              mVisible{true}
        {
        }

        std::string mId;

        const Object* mParent;

        std::string mPath;

        PositionDimension mPositionDimension;
        Margin mMargin;

        HorizontalAlignment mHorizontalAlignment;
        VerticalAlignment mVerticalAlignment;

        std::vector<ObjectSharedPtr> mChildren;

        bool mVisible;

        friend void DrawDebug();
    };

    void RemoveAllObjects();
    void AddRootObject(const ObjectSharedPtr& object);
    void DrawImguiObjectTreeDebugMenu(const bool forceExpand);

    Object* FindObjectByPath(std::string_view path);
} // namespace UI
