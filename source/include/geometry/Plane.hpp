#pragma once
#include "../math/vec/vec.hpp"
#include "Object.hpp"
#include <memory>
#include <optional>


class Plane : public Object
{
    private:

        float z_level;
        float min_x;
        float max_x;
        float min_y;
        float max_y;
        vec3 normal;
        std::shared_ptr <BSDF> material;
        std::optional <Emitter> emitter = std::nullopt;

    public:

        Plane (const float z_position, const float min_x_pos, const float max_x_pos, const float min_y_pos, const float max_y_pos, const vec3& normal_vec,
               const std::shared_ptr <BSDF>& mat);

        Plane (const float z_position, const float min_x_pos, const float max_x_pos, const float min_y_pos, const float max_y_pos, const vec3& normal_vec,
               const Emitter& emitter_info);
    

        bool intersect (ray& ray) const override;
};
