#include "ray/ray.hpp"
#include "material/Diffuse.hpp"
#include "math/random/random.hpp"

Diffuse::Diffuse () : color (1.0f, 1.0f, 1.0f) {}
Diffuse::Diffuse (const vec3& color_value) : color (color_value) {}

void Diffuse::scatter (ray& ray) const
{
    vec3 dir = ray.direction;
    vec3 contrib = ray.pixel_color_contribution;

    if (dir.dot (ray.info.normal) > 0.0f)
    {
        ray.info.normal = -ray.info.normal;
    }

    contrib *= color;
    ray.origin = ray.info.point.add (ray.info.normal.mul (0.0001f));
    vec3 rand_vec = ray.info.normal.add (random_sphere_vector_normalized ()).normalized ();

    if (rand_vec.dot (ray.info.normal) < 0.0f)
    {
        rand_vec = -rand_vec;
    }

    ray.direction = rand_vec;
    ray.pixel_color_contribution = contrib;
    ray.bounces ++;
    ray.info.distance = 1e6f;
}
