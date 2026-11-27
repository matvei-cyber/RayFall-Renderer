#pragma once
#include "../ray/ray.hpp"


class Object
{
    public:

        virtual ~Object () = default;
        virtual bool intersect (ray& ray) const = 0;
};
