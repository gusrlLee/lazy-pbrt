#ifndef GI_CORE_TYPES_HPP
#define GI_CORE_TYPES_HPP

#include <iostream>
#include <stdio.h>

namespace gi
{
    typedef int Int;
    typedef unsigned int UInt;
    typedef unsigned int Index;

#if 0
    typedef double Float;
#else
    typedef float Float;
#endif

    typedef std::string String;
    typedef const char CString;
    typedef size_t Size;
    typedef bool Bool;

    const Float INF = std::numeric_limits<Float>::infinity();
    const Float PI = 3.1415926535897932385;
    const Float EPS = std::numeric_limits<Float>::epsilon();

    template <typename T>
    using SPtr = std::shared_ptr<T>;

    template <typename T, typename... Args>
    constexpr SPtr<T> MakeShared(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using UPtr = std::unique_ptr<T>;

    template <typename T, typename... Args>
    constexpr UPtr<T> MakeUnique(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}

#endif // GI_CORE_TYPES_HPP
