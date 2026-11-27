#include "ray/ray.hpp"
#include "material/Diffuse.hpp"
#include "math/random/random.hpp"


Diffuse::Diffuse ()
{
    albedo = vec3 {0.85f, 0.85f, 0.85f};
}

Diffuse::Diffuse (const vec3& color_value)
{
    albedo = color_value;
}


void Diffuse::scatter (ray& ray, std::mt19937& gen, std::uniform_real_distribution <float>& dist) const
{
    vec3 dir = ray.get_direction ();
    vec3 contrib = ray.get_pixel_color_contribution ();
    vec3 origin = ray.get_origin ();
    int bounces = ray.get_bounces ();

    if (dir.dot (ray.info.normal) > 0.0f)
    {
        ray.info.normal = -ray.info.normal;
    }

    contrib *= albedo;
    origin = ray.info.point.add (ray.info.normal.mul (0.0001f));
    vec3 rand_vec = cosine_random_sphere_vector_normalized (ray.info.normal, gen, dist);

    if (rand_vec.dot (ray.info.normal) < 0.0f)
    {
        rand_vec = -rand_vec;
    }

    ray.set_direction (rand_vec);
    ray.set_pixel_color_contribution (contrib);
    ray.set_bounces (bounces += 1);
    ray.set_origin (origin);
    ray.info.distance = 1e6f;
    ray.info.emitter = nullptr;
}
