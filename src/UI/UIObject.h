#pragma once

#include <string>
#include <vector>

#include "Common/Color.h"
#include "Common/Texture.h"

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
        void SetParent(Object* parent) { mParent = parent; }
        virtual void Update();

        virtual void DrawImguiObjectTreeDebugMenu() const;
        virtual void DrawImguiObjectDetailsDebugMenu() const;

        friend void DrawDebug();

        void AddChild(ObjectPtr object);

    protected:
        Object(const std::string& id) : mId(id) {}

        void DrawChildren() const;

        const std::string mId;

        PositionDimension mPositionDimension = {0.0f, 0.0f, 0.0f, 0.0f};
        Margin mMargin                       = {0.0f, 0.0f, 0.0f, 0.0f};
        Object* mParent                      = nullptr;

        std::vector<ObjectPtr> mChildren;
    };

    class RootObject final : public Object
    {
        DECLARE_UI_ELEMENT(RootObject);

    public:
        void DrawImguiObjectTreeDebugMenu() const override;

        void Draw() const override;
        void Update() override;

        using Object::AddChild;

        friend RootObject& Root();
        friend void Draw();
    };

    class Material : public Object
    {
        DECLARE_UI_ELEMENT(Material);

    public:
        void SetImagePath(const std::string& imagePath);
        void SetColor(Color color) { mColor = color; }

    protected:
        virtual void Draw() const override;

    private:
        virtual void DrawImguiObjectDetailsDebugMenu() const;

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
        void SetColor(Color color) { mColor = color; }

    protected:
        virtual void Draw() const override;

    private:
        TTF_Font* mFont = nullptr;

        TextureUniquePtr mTexture = Common::EmptyTexture();

        Color mColor = Common::Color::BLACK;

        std::string mFontPath;
        std::string mText;
    };

    RootObject& Root();

} // namespace UI
