#pragma once
#include "../ray/ray.hpp"
#include "Object.hpp"
#include <memory>
#include <optional>


struct Sphere : Object
{
    vec3 origin;
    float radius;
    std::shared_ptr <BSDF> material;
    std::optional <Emitter> emitter = std::nullopt;


    Sphere (const vec3& origin_position, const float radius_value, const std::shared_ptr <BSDF>& mat);
    Sphere (const vec3& origin_position, const float radius_value, const Emitter& emitter_info);


    vec3 normal (const vec3& point) const;
    bool intersect (ray& ray) override;
    vec3 random_point () override;
};
