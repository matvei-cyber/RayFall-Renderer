#pragma once
#include "../ray/ray.hpp"


struct Glossy : BSDF
{
    vec3 color;
    float roughness;


    Glossy (const vec3& color_value, const float roughness_value);


    void scatter (ray& ray, std::mt19937& gen, std::uniform_real_distribution <float>& dist) const;
};
