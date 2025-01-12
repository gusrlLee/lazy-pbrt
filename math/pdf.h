#pragma once 

#include "core/onb.h"

class Pdf 
{
public:
    virtual ~Pdf() {}

    virtual F32 Value(const Vec3 &dir) const = 0;
    virtual Vec3 Gen() const = 0;
};