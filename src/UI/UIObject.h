#pragma once

#include <string>
#include <vector>

#include "Common/Color.h"
#include "Common/Texture.h"

#include "Tools/ImguiDebug.h"

#define DECLARE_UI_ELEMENT_DERIVED(UIClassName, UIClassNameParent)                                                     \
public:                                                                                                                \
    using superclass = UIClassNameParent;                                                                              \
    UIClassName##(const std::string& id) : UIClassNameParent##(id) {}                                                  \
    static auto Make(const std::string& id) { return std::unique_ptr<UIClassName##>(new UIClassName##(id)); }

#define DECLARE_UI_ELEMENT(UIClassName) DECLARE_UI_ELEMENT_DERIVED(##UIClassName, Object)

namespace UI
{
    using ObjectPtr = std::unique_ptr<class Object>;

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
    public:
        Object(const Object&)            = delete;
        Object& operator=(const Object&) = delete;
        virtual ~Object();

        virtual void Draw() const = 0;

        void SetHeight(float height) { mPositionDimension.h = height; }
        void SetWidth(float width) { mPositionDimension.w = width; }
        void SetMargin(Margin margin) { mMargin = margin; }
        void SetParent(Object& parent);
        void SetVisibility(bool visibility) { mVisibility = visibility; }

        bool GetVisibility() const { return mVisibility; }

        virtual void Update();

        virtual void DrawImguiObjectTreeDebugMenu();
        virtual void DrawImguiObjectDetailsDebugMenu();

        friend void DrawDebug();

        void UpdatePath();
        void AddChild(ObjectPtr object);
        void RemoveChild(std::string_view childId);
        void RemoveChild(int childIndex);

    protected:
        Object(const std::string& id) : mId(id), mPath(id) {}

        void DrawChildren() const;

        const std::string mId;

        PositionDimension mPositionDimension = {0.0f, 0.0f, 0.0f, 0.0f};
        Margin mMargin                       = {0.0f, 0.0f, 0.0f, 0.0f};
        bool mVisibility                     = true;

        const Object* mParent = nullptr;

        std::string mPath;
        std::vector<ObjectPtr> mChildren;
    };

    class RootObject final : private Object
    {
        DECLARE_UI_ELEMENT(RootObject);

    public:
        using Object::AddChild;

    private:
        void DrawImguiObjectTreeDebugMenu() override;

        void Draw() const override;
        void Update() override;

        friend RootObject& Root();
        friend void Draw();
        friend void Update();
        friend void ImguiDebug::DrawMenus();
    };

    class Material : public Object
    {
        DECLARE_UI_ELEMENT(Material);

    public:
        void SetImagePath(const std::string& imagePath);
        void SetColor(Color color) { mColor = color; }

    private:
        virtual void Draw() const override;

        virtual void DrawImguiObjectDetailsDebugMenu() override;

        TextureUniquePtr mTexture = Common::EmptyTexture();
        Color mColor              = Common::Color::TRANSPARENT;
        std::string mTexturePath;
    };

    class Text : public Object
    {
        DECLARE_UI_ELEMENT(Text);

    public:
        void SetFontPath(const std::string& fontPath);
        void SetText(const std::string& text);
        void SetColor(Color color);

    private:
        void UpdateTexture();

        virtual void Draw() const override;
        virtual void DrawImguiObjectDetailsDebugMenu() override;

        TTF_Font* mFont = nullptr;

        TextureUniquePtr mTexture = Common::EmptyTexture();

        Color mColor = Common::Color::BLACK;

        std::string mFontPath;
        std::string mText;
    };

    RootObject& Root();

} // namespace UI
