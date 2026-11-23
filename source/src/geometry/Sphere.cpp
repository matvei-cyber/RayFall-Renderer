#include "geometry/Object.hpp"
#include "geometry/Sphere.hpp"
#include "math/random/random.hpp"
#include <cmath>


Sphere::Sphere (const vec3& origin_position, const float radius_value, const std::shared_ptr <BSDF>& mat):
origin (origin_position), radius (radius_value), material (std::move (mat)) {}

Sphere::Sphere (const vec3& origin_position, const float radius_value, const Emitter& emitter_info):
origin (origin_position), radius (radius_value), emitter (emitter_info) {}


vec3 Sphere::normal (const vec3& point) const
{
    vec3 P = point;
    vec3 O = origin;
    vec3 N = P.sub (O).normalized ();
    return N;
}

bool Sphere::intersect (ray& ray)
{
    vec3 O = ray.origin;
    vec3 C = origin;
    vec3 L = O.sub (C);
    vec3 D = ray.direction;
    float R = radius;

    float b = L.dot (D);
    float disc = (b * b) - (L.dot (L) - (R * R));

    if (disc < 0.0f)
    {
        return false;
    }

    float LD = -(L.dot (D));
    float disc_sq = std::sqrtf (disc);

    float t1 = LD + disc_sq;
    float t2 = LD - disc_sq;

    if ((t1 >= 0.0f) && (t2 >= 0.0f))
    {
        float dist = std::min (t1, t2);
        vec3 I = O.add (D.mul (dist));

        if (dist < ray.info.distance)
        {
            ray.info.point = I;
            ray.info.distance = dist;
            ray.info.normal = normal (I);

            if (emitter.has_value ())
            {
                ray.info.emitter = &(*this -> emitter);

            } else
            {
                ray.info.material = material.get ();
                ray.info.emitter = nullptr;
            }

            return true;
        }

    } else if ((t1 >= 0.0f) && (t2 < 0.0f))
    {
        float dist = t1;
        vec3 I = O.add (D.mul (dist));

        if (dist < ray.info.distance)
        {
            ray.info.point = I;
            ray.info.distance = dist;
            ray.info.normal = normal (I);

            if (emitter.has_value ())
            {
                ray.info.emitter = &(*this -> emitter);

            } else
            {
                ray.info.material = material.get ();
                ray.info.emitter = nullptr;
            }

            return true;
        }

    } else if ((t1 < 0.0f) && (t2 >= 0.0f))
    {
        float dist = t2;
        vec3 I = O.add (D.mul (dist));

        if (dist < ray.info.distance)
        {
            ray.info.point = I;
            ray.info.distance = dist;
            ray.info.normal = normal (I);

            if (emitter.has_value ())
            {
                ray.info.emitter = &(*this -> emitter);

            } else
            {
                ray.info.material = material.get ();
                ray.info.emitter = nullptr;
            }

            return true;
        }
    }

    return false;
}

vec3 Sphere::random_point ()
{
    return origin.add (random_sphere_vector_normalized ());
}
