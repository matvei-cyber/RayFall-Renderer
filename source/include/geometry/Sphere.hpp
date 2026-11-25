#pragma once
#include "../ray/ray.hpp"
#include "Object.hpp"
#include <memory>
#include <optional>


class Sphere : public Object
{
    private:

        vec3 origin;
        float radius;
        std::shared_ptr <BSDF> material;
        std::optional <Emitter> emitter = std::nullopt;

    public:

        Sphere (const vec3& origin_position, const float radius_value, const std::shared_ptr <BSDF>& mat);
        Sphere (const vec3& origin_position, const float radius_value, const Emitter& emitter_info);

        bool intersect (ray& ray) const override;
        vec3 normal (const vec3& point) const ;
};
