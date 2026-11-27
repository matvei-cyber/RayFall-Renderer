#pragma once
#include "../ray/ray.hpp"
#include "Object.hpp"


class Sphere : public Object
{
    private:

        vec3 origin;
        float radius;
        BSDF* material = nullptr;
        Emitter* emitter = nullptr;

    public:

        Sphere ();
        Sphere (const vec3& origin_position, const float radius_value, BSDF* mat);
        Sphere (const vec3& origin_position, const float radius_value, Emitter* emitter_info);


        vec3 get_origin () const;
        float get_radius () const;

        void set_origin (const vec3& new_origin);
        void set_radius (const float new_radius);
        void set_BSDF (BSDF* new_mat);
        void set_emitter (Emitter* new_emitter);

        bool intersect (ray& ray) const override;
        vec3 normal (const vec3& point) const;
};
