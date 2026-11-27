#include "ray/ray.hpp"
#include "material/Diffuse.hpp"
#include "math/random/random.hpp"


Diffuse::Diffuse ()
{
    color = vec3 {0.85f, 0.85f, 0.85f};
}

Diffuse::Diffuse (const vec3& color_value)
{
    color = color_value;
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

    contrib *= color;
    origin = ray.info.point.add (ray.info.normal.mul (0.0001f));
    vec3 rand_vec = cosine_random_vector_nomralized (ray.info.normal, gen, dist);

    if (rand_vec.dot (ray.info.normal) < 0.0f)
    {
        rand_vec = -rand_vec;
    }

    ray.write_direction (rand_vec);
    ray.write_pixel_color_contribution (contrib);
    ray.write_bounces (bounces += 1);
    ray.write_origin (origin);
    ray.info.distance = 1e6f;
    ray.info.emitter = nullptr;
}
