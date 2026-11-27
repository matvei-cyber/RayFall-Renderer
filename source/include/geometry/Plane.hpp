#pragma once
#include "Object.hpp"


class Plane : public Object
{
    private:

        float z_level;
        float min_x;
        float max_x;
        float min_y;
        float max_y;
        vec3 normal;
        BSDF* material = nullptr;
        Emitter* emitter = nullptr;

    public:

        Plane ();

        Plane (const float z_position, const float min_x_pos, const float max_x_pos, const float min_y_pos, const float max_y_pos, const vec3& normal_vec,
               BSDF* mat);

        Plane (const float z_position, const float min_x_pos, const float max_x_pos, const float min_y_pos, const float max_y_pos, const vec3& normal_vec,
               Emitter* emitter_info);
    

        void set_z_level (const float new_z_level);
        void set_min_x (const float new_min_x);
        void set_max_x (const float new_max_x);
        void set_min_y (const float new_min_y);
        void set_max_y (const float new_max_y);
        void set_normal (const vec3& new_normal);
        void set_material (BSDF* new_material);
        void set_emitter (Emitter* new_emitter);

        bool intersect (ray& ray) const override;
};
