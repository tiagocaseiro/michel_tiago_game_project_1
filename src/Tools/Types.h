#pragma once

#include <string>

namespace TypeUtils
{
    template <typename T>
    std::string get_type_name()
    {
#if defined(_MSC_VER)
        const std::string func_sig = __FUNCSIG__;
        const std::string prefix   = "get_type_name<";
        const std::string suffix   = ">(void)";
#elif defined(__clang__) || defined(__GNUC__)
        const std::string func_sig = __PRETTY_FUNCTION__;
        const std::string prefix   = "get_type_name() [with T = ";
        const std::string suffix   = "]";
#else
        return "Unsupported compiler";
#endif

        auto start = func_sig.find(prefix) + prefix.size();
        auto end   = func_sig.rfind(suffix);

        return func_sig.substr(start, end - start);
    }
} // namespace TypeUtils