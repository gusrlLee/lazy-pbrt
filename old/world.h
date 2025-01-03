#pragma once
#include "types.h"
#include "vec3.h"
#include "ray.h"
#include "hit_record.h"
#include "primitive.h"
#include "interval.h"

#include <memory>
#include <vector>

class World : public Primitive
{
public:
    std::vector<std::shared_ptr<Primitive>> objects;

    World() {}
    World(std::shared_ptr<Primitive> object) { add(object); }
    void clear() { objects.clear(); }
    void add(std::shared_ptr<Primitive> object) { objects.push_back(object); }
    
    bool hit(const Ray &r, Interval r_t, hit_record &rec) const override;
};
