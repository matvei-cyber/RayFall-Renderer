#pragma once
#include "../math/vec/vec.hpp"
#include "../light/emitter.hpp"


struct BSDF;

struct hit_info
{
    float distance = 1e6f;
    vec3 point;
    vec3 normal;
    vec3 color;
    const BSDF* material = nullptr;
    const Emitter* emitter = nullptr;
};

struct ray
{
    vec3 origin;
    vec3 direction;
    vec3 pixel_color_contribution = {1.0f, 1.0f, 1.0f};
    hit_info info;
    int bounces = 0;

    ray ();
    ray (const vec3& origin_position, const vec3& direction_vector);

    vec3 reflected ();
};

struct BSDF
{
    virtual ~BSDF () = default;
    virtual void scatter (ray& ray) const = 0;
};
