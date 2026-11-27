#include "geometry/Sphere.hpp"
#include "math/random/random.hpp"
#include "material/Diffuse.hpp"
#include <cmath>


Sphere::Sphere () : origin (vec3 {}), radius (1.0f)
{
    BSDF* mat = new Diffuse (vec3 {0.85f, 0.85f, 0.85f});
    material = mat;
}

Sphere::Sphere (const vec3& origin_position, const float radius_value, BSDF* mat):
origin (origin_position), radius (radius_value), material (mat) {}

Sphere::Sphere (const vec3& origin_position, const float radius_value, Emitter* emitter_info):
origin (origin_position), radius (radius_value), emitter (emitter_info) {}


vec3 Sphere::normal (const vec3& point) const
{
    vec3 P = point;
    vec3 O = origin;
    vec3 N = P.sub (O).normalized ();
    return N;
}

bool Sphere::intersect (ray& ray) const
{
    vec3 O = ray.get_origin ();
    vec3 C = origin;
    vec3 L = O.sub (C);
    vec3 D = ray.get_direction ();
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

            if (emitter != nullptr)
            {
                ray.info.emitter = this -> emitter;

            } else
            {
                ray.info.material = material;
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

            if (emitter != nullptr)
            {
                ray.info.emitter = this -> emitter;

            } else
            {
                ray.info.material = material;
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

            if (emitter != nullptr)
            {
                ray.info.emitter = this -> emitter;

            } else
            {
                ray.info.material = material;
                ray.info.emitter = nullptr;
            }

            return true;
        }
    }

    return false;
}

vec3 Sphere::get_origin () const
{
    return origin;
}

float Sphere::get_radius () const
{
    return radius;
}

void Sphere::set_origin (const vec3& new_origin)
{
    origin = new_origin;
}

void Sphere::set_radius (const float new_radius)
{
    if (new_radius > 0.001f)
    {
        radius = new_radius;
    }
}

void Sphere::set_BSDF (BSDF* new_mat)
{ 
    material = new_mat;
}

void Sphere::set_emitter (Emitter* new_emitter)
{
    emitter = new_emitter;
}
