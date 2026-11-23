#pragma once
#include "../ray/ray.hpp"
#include "../material/Diffuse.hpp"

struct Diffuse : BSDF
{
    vec3 color;

    Diffuse ();
    Diffuse (const vec3& color_value);

    void scatter (ray& ray) const override;
};
