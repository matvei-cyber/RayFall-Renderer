#include "geometry/Plane.hpp"
#include "material/Diffuse.hpp"


Plane::Plane () : z_level (0.0f), min_x (-1.0f), max_x (1.0f), min_y (-1.0f), max_y (1.0f), normal (0.0f, 0.0f, 1.0f)
{
    BSDF* mat = new Diffuse (vec3 {0.85f, 0.85f, 0.85f});
    material = mat;
}

Plane::Plane (const float z_position, const float min_x_pos, const float max_x_pos, const float min_y_pos, const float max_y_pos, const vec3& normal_vec,
        BSDF* mat): z_level (z_position), min_x (min_x_pos), max_x (max_x_pos), min_y (min_y_pos), max_y (max_y_pos),
        normal (normal_vec), material (mat) {}

Plane::Plane (const float z_position, const float min_x_pos, const float max_x_pos, const float min_y_pos, const float max_y_pos, const vec3& normal_vec,
        const Emitter& emitter_info): z_level (z_position), min_x (min_x_pos), max_x (max_x_pos), min_y (min_y_pos), max_y (max_y_pos),
        normal (normal_vec), emitter (emitter_info) {}


bool Plane::intersect (ray& ray) const
{
    float p_z = z_level;
    float ray_o_z = ray.get_origin ().z;
    float ray_d_z = ray.get_direction ().z;
    vec3 origin = ray.get_origin ();
    vec3 dir = ray.get_direction ();

    if (ray_d_z == 0.0f)
    {
        return false;
    }

    float t = (p_z - ray_o_z) / ray_d_z;

    if (t >= 0.0f)
    {
        vec3 I = origin.add (dir.mul (t));

        if (I.x > min_x && I.x < max_x && I.y > min_y && I.y < max_y && (t < ray.info.distance))
        {
            ray.info.point = I;
            ray.info.distance = t;
            ray.info.normal = normal;

            if (emitter.has_value ())
            {
                ray.info.emitter = &(*this -> emitter);

            } else
            {
                ray.info.emitter = nullptr;
                ray.info.material = material;
            }

            return true;
        }
    }

    return false;
}


void Plane::set_z_level (const float new_z_level)
{
    z_level = new_z_level;
}

void Plane::set_min_x (const float new_min_x)
{
    min_x = new_min_x;
}

void Plane::set_max_x (const float new_max_x)
{
    max_x = new_max_x;
}

void Plane::set_min_y (const float new_min_y)
{
    min_y = new_min_y;
}

void Plane::set_max_y (const float new_max_y)
{
    max_y = new_max_y;
}

void Plane::set_normal (const vec3& new_normal)
{
    vec3 new_normal_normalized = new_normal.normalized ();

    if (new_normal_normalized.dot (new_normal_normalized) < 0.9999f)
    {
        normal = {0.0f, 0.0f, 1.0f};

    } else
    {
        normal = new_normal_normalized;
    }
}

void Plane::set_material (BSDF* new_material)
{
    material = new_material;
}

void Plane::set_emitter (const std::optional <Emitter>& new_emitter_info)
{
    emitter = new_emitter_info;
}
