#pragma once
#include "../math/vec/vec.hpp"
#include "../light/emitter.hpp"
#include <random>


struct BSDF;

struct hit_info
{
    float distance = 1e6f;
    vec3 point;
    vec3 normal;
    const BSDF* material = nullptr;
    const Emitter* emitter = nullptr;
};

class ray
{
    private:

        vec3 origin;
        vec3 direction;
        vec3 pixel_color_contribution = {1.0f, 1.0f, 1.0f};
        int bounces = 0;

    public:

        hit_info info;


        ray ();
        ray (const vec3& origin_position, const vec3& direction_vector);


        vec3 get_origin () const;
        vec3 get_direction () const;
        vec3 get_pixel_color_contribution () const;
        int get_bounces () const;

        void set_origin (const vec3& new_origin);
        void set_direction (const vec3& new_direction);
        void set_pixel_color_contribution (const vec3& new_pixel_color_contribution);
        void set_bounces (const int new_bounces);

        vec3 reflected () const;
};

struct BSDF
{
    vec3 albedo;


    virtual ~BSDF () = default;
    virtual void scatter (ray& ray, std::mt19937& gen, std::uniform_real_distribution <float>& dist) const = 0;
};
