#pragma once
#include "../ray/ray.hpp"


struct Object
{
    virtual ~Object () = default;
    virtual bool intersect (ray& ray) = 0;
};
