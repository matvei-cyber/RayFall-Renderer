#include "geometry/Plane.hpp"


Plane::Plane (const float z_position, const float min_x_pos, const float max_x_pos, const float min_y_pos, const float max_y_pos, const vec3& normal_vec,
        const std::shared_ptr <BSDF>& mat): z_level (z_position), min_x (min_x_pos), max_x (max_x_pos), min_y (min_y_pos), max_y (max_y_pos),
        normal (normal_vec), material (std::move (mat)) {}

Plane::Plane (const float z_position, const float min_x_pos, const float max_x_pos, const float min_y_pos, const float max_y_pos, const vec3& normal_vec,
        const Emitter& emitter_info): z_level (z_position), min_x (min_x_pos), max_x (max_x_pos), min_y (min_y_pos), max_y (max_y_pos),
        normal (normal_vec), emitter (emitter_info) {}


bool Plane::intersect (ray& ray) const
{
    float p_z = z_level;
    float ray_o_z = ray.get_origin ().z;
    float ray_d_z = ray.get_direction ().z;
    vec3 origin = ray.get_origin ();
    vec3 dir = ray.get_direction ();

    if (ray_d_z == 0.0f)
    {
        return false;
    }

    float t = (p_z - ray_o_z) / ray_d_z;

    if (t >= 0.0f)
    {
        vec3 I = origin.add (dir.mul (t));

        if (I.x > min_x && I.x < max_x && I.y > min_y && I.y < max_y && (t < ray.info.distance))
        {
            ray.info.point = I;
            ray.info.distance = t;
            ray.info.normal = normal;

            if (emitter.has_value ())
            {
                ray.info.emitter = &(*this -> emitter);

            } else
            {
                ray.info.emitter = nullptr;
                ray.info.material = material.get ();
            }

            return true;
        }
    }

    return false;
}
