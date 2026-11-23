#include "ray/ray.hpp"
#include "material/Glossy.hpp"
#include "math/random/random.hpp"


Glossy::Glossy (const vec3& color_value, const float roughness_value) : color (color_value), roughness (roughness_value) {}


void Glossy::scatter (ray& ray) const
{
    if (ray.direction.dot (ray.info.normal) > 0.0f)
    {
        ray.info.normal = -ray.info.normal;
    }

    vec3 reflected = ray.reflected ();
    vec3 diffuse_dir = ray.info.normal.add (random_sphere_vector_normalized ()).normalized ();

    if (diffuse_dir.dot (ray.info.normal) < 0.0f)
    {
        diffuse_dir = -diffuse_dir;
    }

    float alpha = roughness * roughness;

    vec3 reflected_component = reflected.mul (1.0f - alpha);
    vec3 diffuse_component = diffuse_dir.mul (alpha);

    vec3 scattered = reflected_component.add (diffuse_component).normalized ();

    if (scattered.dot (ray.info.normal) <= 0.0f)
    {
        scattered = -scattered;
    }

    ray.pixel_color_contribution *= color;
    ray.origin = ray.info.point.add (ray.info.normal.mul (0.0001f));
    ray.direction = scattered;
    ray.bounces ++;
    ray.info.distance = 1e6f;
}
