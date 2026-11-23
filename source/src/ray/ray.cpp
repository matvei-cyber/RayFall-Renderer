#include "ray/ray.hpp"


ray::ray () : origin (vec3 {}), direction (vec3 {1.0f, 0.0f, 0.0f}) {}
ray::ray (const vec3& origin_position, const vec3& direction_vector) : origin (origin_position), direction (direction_vector) {}


vec3 ray::reflected ()
{
    return direction.sub (info.normal.mul (2.0f * direction.dot (info.normal))).normalized ();
}
