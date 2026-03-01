#pragma once

// A strongly-typed Globally Unique Identifier
// Very lightweight class. Same as using an int, just ensures the wrong type isn't passed
template<typename  T>
class TGUID
{
public:
    explicit TGUID(const int guid)
        : mGuid(guid)
    {

    }

    static TGUID Generate()
    {
        static int total = 0;
        return TGUID(++total);
    }

    [[nodiscard]] int ID() const { return mGuid; }

private:
    int mGuid;
};