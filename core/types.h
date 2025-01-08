#pragma once 

#include <iostream>
#include <stdio.h>

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef size_t Size;

typedef float F32;
typedef double F64;
typedef std::string Str;
typedef const char Cstr;

const F32 inf = std::numeric_limits<F32>::infinity();
const F32 pi = 3.1415926535897932385;

inline F32 DegreeToRadians(F32 degrees)
{
    return degrees * pi / 180.0;
}

template <typename T>
using Sptr = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Sptr<T> MakeSptr(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
using Uptr = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Uptr<T> MakeUptr(Args &&...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}