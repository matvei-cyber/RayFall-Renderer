#pragma once
#include "../ray/ray.hpp"


struct Diffuse : BSDF
{
    Diffuse ();
    Diffuse (const vec3& color_value);


    void scatter (ray& ray, std::mt19937& gen, std::uniform_real_distribution <float>& dist) const;
};
