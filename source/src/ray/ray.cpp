#include "ray/ray.hpp"


ray::ray () : origin (vec3 {}), direction (vec3 {1.0f, 0.0f, 0.0f}) {}
ray::ray (const vec3& origin_position, const vec3& direction_vector) : origin (origin_position), direction (direction_vector) {}


vec3 ray::get_origin () const
{
    return origin;
}

vec3 ray::get_direction () const
{
    return direction;
}

vec3 ray::get_pixel_color_contribution () const
{
    return pixel_color_contribution;
}

int ray::get_bounces () const
{
    return bounces;
}

void ray::set_origin (const vec3& new_origin)
{
    origin = new_origin;
}

void ray::set_direction (const vec3& new_direction)
{
    if (new_direction.dot (new_direction) != 1.0f)
    {
        direction = new_direction.normalized ();
    }
}

void ray::set_pixel_color_contribution (const vec3& new_pixel_color_contribution)
{
    pixel_color_contribution = new_pixel_color_contribution;
}

void ray::set_bounces (const int new_bounces)
{
    if (new_bounces >= bounces)
    {
        bounces = new_bounces;
    }
}

vec3 ray::reflected () const
{
    return direction.sub (info.normal.mul (2.0f * direction.dot (info.normal))).normalized ();
}
