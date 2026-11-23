#pragma once
#include "../ray/ray.hpp"
#include "../material/Glossy.hpp"


struct Glossy : BSDF
{
    vec3 color;
    float roughness;


    Glossy (const vec3& color_value, const float roughness_value);


    void scatter (ray& ray) const override;
};
