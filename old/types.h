#pragma once

#include <iostream>
#include <stdio.h>
#include <random>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef size_t size;

typedef float f32;
typedef double f64;
typedef std::string str;
typedef const char cstr;

const f64 inf = std::numeric_limits<f64>::infinity();
const f64 pi = 3.1415926535897932385;

inline f64 random_value()
{
    static std::uniform_real_distribution<f64> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline f64 random_value(f64 min, f64 max)
{ 
    // Returns a random real in [min,max).
    return min + (max - min) * random_value();
}

inline f64 degrees_to_radians(f64 degrees)
{
    return degrees * pi / 180.0;
}