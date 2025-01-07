#pragma once 

#include "core/ray.h"
#include "core/hitrecord.h"
#include "core/hitable.h"

class HitTableList : public HitTable
{
public:
    std::vector<std::shared_ptr<HitTable>> objects;

    HitTableList() {}
    HitTableList(std::shared_ptr<HitTable> object) { Add(object); }

    void Clear() { objects.clear(); }
    void Add(std::shared_ptr<HitTable> object) { objects.push_back(object); }
    bool Hit(const Ray &r, Interval t, HitRecord &rec) const override;
};