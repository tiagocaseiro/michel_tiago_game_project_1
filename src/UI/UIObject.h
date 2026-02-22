#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <SDL3/SDL.h>

#define DECLARE_UI_ELEMENT_DERIVED(UIClassName, UIClassNameParent)                                                     \
public:                                                                                                                \
    using superclass = UIClassNameParent;                                                                              \
    UIClassName##(const std::string& id) : UIClassNameParent##(id) {}                                                  \
    friend void DrawDebug();                                                                                           \
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

    void SetRenderer(SDL_Renderer* renderer);
    void Update();
    void Draw();
    void DrawDebug();

    class Object
    {
    public:
        Object(const Object&)            = delete;
        Object& operator=(const Object&) = delete;
        virtual ~Object();

        virtual void Draw() const;

        void SetHeight(float height) { mPositionDimension.h = height; }
        void SetWidth(float width) { mPositionDimension.w = width; }
        void SetMargin(Margin margin) { mMargin = margin; }
        void SetColor(Color color) { mColor = color; }
        void SetParent(Object* parent) { mParent = parent; }

        virtual void Update();

        virtual void DrawImguiObjectTreeDebugMenu() const;
        virtual void DrawImguiObjectDetailsDebugMenu() const;

        friend void DrawDebug();

        void AddChild(ObjectPtr object);

        static auto Make(const std::string& id) { return std::unique_ptr<Object>(new Object(id)); }

    protected:
        Object(const std::string& id) : mId(id) {}

        const std::string mId;

        PositionDimension mPositionDimension = {0.0f, 0.0f, 0.0f, 0.0f};
        Color mColor                         = {0.0f, 0.0f, 0.0f, 0.0f};
        Margin mMargin                       = {0.0f, 0.0f, 0.0f, 0.0f};

        std::vector<ObjectPtr> mChildren;

        Object* mParent;
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

    class Image : public Object
    {
        DECLARE_UI_ELEMENT(Image);

        ~Image();

    public:
        void SetImagePath(const std::string& imagePath);

    protected:
        virtual void Draw() const override;

    private:
        virtual void DrawImguiObjectDetailsDebugMenu() const;

        std::string mImagePath;
        SDL_Texture* mTexture = nullptr;
    };

    class Text : public Object
    {
        DECLARE_UI_ELEMENT(Text);

    protected:
        virtual void Draw() const override;

    private:
        std::string mText;
    };

    RootObject& Root();

} // namespace UI
