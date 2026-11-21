#pragma once
#include <cmath>
#include <memory>
#include <random>
#include <optional>
#include "raylib.h"
inline const float pi = std::acos (-1.0f);


inline float deg_to_rad (const float deg)
{
    return (deg * pi) / 180.0f;
}

struct vec2
{
    float x, y;

    vec2 () : x (0.0f), y (0.0f) {}
    vec2 (const float x_value, const float y_value) : x (x_value), y (y_value) {}

    vec2 add (const vec2& other) const
    {
        return vec2 {x + other.x, y + other.y};
    }

    void add_rw (const vec2& other)
    {
        x += other.x;
        y += other.y;
    }

    vec2 sub (const vec2& other) const
    {
        return vec2 {x - other.x, y - other.y};
    }

    void sub_rw (const vec2& other)
    {
        x -= other.x;
        y -= other.y;
    }

    vec2 operator- () const
    {
        return vec2 {-x, -y};
    }

    vec2 div (const float num) const
    {
        if (num == 0.0f)
        {
            return *this;

        } else
        {
            return vec2 {x / num, y / num};
        }
    }

    void div_rw (const float num)
    {
        if (num == 0.0f)
        {
            return;

        } else
        {
            x /= num;
            y /= num;
        }
    }

    vec2 mul (const float num) const
    {
        return vec2 {x * num, y * num};
    }

    void mul_rw (const float num)
    {
        x *= num;
        y *= num;
    }

    vec2 operator* (const vec2& other) const
    {
        return vec2 {x * other.x, y * other.y};
    }

    void operator*= (const vec2& other)
    {
        x *= other.x;
        y *= other.y;
    }

    float dot (const vec2& other) const
    {
        return (x * other.x) + (y * other.y);
    }

    float length () const
    {
        return std::sqrtf ((x * x) + (y * y));
    }

    vec2 normalized () const
    {
        if (float len = length (); len == 0.0f)
        {
            return *this;

        } else
        {
            return this -> div (len);
        }
    }

    void normalize ()
    {
        if (float len = length (); len == 0.0f)
        {
            return;

        } else
        {
            this -> div_rw (len);
        }
    }

    vec2 right_normal () const
    {
        return vec2 {y, -x};
    }

    vec2 left_normal () const
    {
        return vec2 {-y, x};
    }
};

struct vec3
{
    float x, y, z;

    vec3 () : x (0.0f), y (0.0f), z (0.0f) {}
    vec3 (const float x_value, const float y_value, const float z_value) : x (x_value), y (y_value), z (z_value) {}

    vec3 add (const vec3& other) const
    {
        return vec3 {x + other.x, y + other.y, z + other.z};
    }

    void add_rw (const vec3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    vec3 sub (const vec3& other) const
    {
        return vec3 {x - other.x, y - other.y, z - other.z};
    }

    void sub_rw (const vec3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    vec3 operator- () const
    {
        return vec3 {-x, -y, -z};
    }

    vec3 div (const float num) const
    {
        if (num == 0.0f)
        {
            return *this;

        } else
        {
            return vec3 {x / num, y / num, z / num};
        }
    }

    void div_rw (const float num)
    {
        if (num == 0.0f)
        {
            return;

        } else
        {
            x /= num;
            y /= num;
            z /= num;
        }
    }

    vec3 mul (const float num) const
    {
        return vec3 {x * num, y * num, z * num};
    }

    void mul_rw (const float num)
    {
        x *= num;
        y *= num;
        z *= num;
    }

    vec3 operator* (const vec3& other) const
    {
        return vec3 {x * other.x, y * other.y, z * other.z};
    }

    void operator*= (const vec3& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
    }

    vec3 cross (const vec3& other) const
    {
        vec3 res;
        res.x = (y * other.z) - (z * other.y);
        res.y = (z * other.x) - (x * other.z);
        res.z = (x * other.y) - (y * other.x);
        return res;
    }

    float dot (const vec3& other) const
    {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }

    float length () const
    {
        return std::sqrtf ((x * x) + (y * y) + (z * z));
    }

    vec3 normalized () const
    {
        if (float len = length (); len == 0.0f)
        {
            return *this;

        } else
        {
            return this -> div (len);
        }
    }

    void normalize ()
    {
        if (float len = length (); len == 0.0f)
        {
            return;

        } else
        {
            this -> div_rw (len);
        }
    }
};

inline vec3 linear_RGB_to_sRGB (const vec3& color)
{
    vec3 new_col;

    if (color.x <= 0.0031308f)
    {
        new_col.x = 12.92f * color.x;

    } else
    {
        new_col.x = (1.055f * std::pow (color.x, (1.0f / 2.4f))) - 0.055f;
    }

    if (color.y <= 0.0031308f)
    {
        new_col.y = 12.92f * color.y;

    } else
    {
        new_col.y = (1.055f * std::pow (color.y, (1.0f / 2.4f))) - 0.055f;
    }

    if (color.z <= 0.0031308f)
    {
        new_col.z = 12.92f * color.z;

    } else
    {
        new_col.z = (1.055f * std::pow (color.z, (1.0f / 2.4f))) - 0.055f;
    }

    return new_col;
}

inline void convert_linear_RGB_to_sRGB (vec3& color)
{
    if (color.x <= 0.0031308f)
    {
        color.x *= 12.92f;

    } else
    {
        color.x = (1.055f * std::pow (color.x, (1.0f / 2.4f))) - 0.055f;
    }

    if (color.y <= 0.0031308f)
    {
        color.y *= 12.92f;

    } else
    {
        color.y = (1.055f * std::pow (color.y, (1.0f / 2.4f))) - 0.055f;
    }

    if (color.z <= 0.0031308f)
    {
        color.z *= 12.92f;

    } else
    {
        color.z = (1.055f * std::pow (color.z, (1.0f / 2.4f))) - 0.055f;
    }
}

inline float random_float ()
{
    thread_local std::mt19937 gen (std::random_device {}());
    thread_local std::uniform_real_distribution <float> dist (-1.0f, 1.0f);
    return dist (gen);
}

inline vec3 random_sphere_vector_normalized ()
{
    float x = random_float ();
    float y = random_float ();
    float z = random_float ();
    vec3 rand_vec = {x, y, z};

    while ((rand_vec.dot (rand_vec) > 1.0f) || ((x == 0.0f) && (y == 0.0f) && (z == 0.0f)))
    {
        x = random_float ();
        y = random_float ();
        z = random_float ();   
        rand_vec = {x, y, z};
    }

    return rand_vec.normalized ();
}

inline float clamp (const float var, const float min_val, const float max_val)
{
    if (var < min_val)
    {
        return min_val;

    } else if (var > max_val)
    {
        return max_val;

    } else
    {
        return var;
    }
}


struct camera3D
{
    vec3 origin;
    vec3 target;
    vec3 direction;
    vec3 right;
    vec3 up;
    float aspect_ratio;
    float horizontal_FOV_degrees;
    float horizontal_FOV;
    float width;
    float height;
    vec3 plane_position;
    float plane_horizontal_size;
    float plane_vertical_size;


    camera3D () : origin (vec3 {}), target (1.0f, 0.0f, 0.0f), horizontal_FOV_degrees (90.0f), width (1920.0f), height (1080.0f)
    {
        horizontal_FOV = deg_to_rad (horizontal_FOV_degrees);
        aspect_ratio = width / height;
        plane_horizontal_size = 2 * std::tanf (horizontal_FOV / 2.0f);
        plane_vertical_size = plane_horizontal_size / aspect_ratio;
        direction = target.sub (origin).normalized ();
        plane_position = origin.add (direction);
        right = direction.cross (vec3 {0.0f, 0.0f, 1.0f}).normalized ();
        up = right.cross (direction).normalized ();
    }

    camera3D (const vec3& origin_position, const vec3& target_position, const float horizontal_FOV_in_degrees, const float screen_width, const float screen_height):
    origin (origin_position), target (target_position), horizontal_FOV_degrees (horizontal_FOV_in_degrees), width (screen_width), height (screen_height)
    {
        horizontal_FOV = deg_to_rad (horizontal_FOV_degrees);
        aspect_ratio = width / height;
        plane_horizontal_size = 2 * std::tanf (horizontal_FOV / 2.0f);
        plane_vertical_size = plane_horizontal_size / aspect_ratio;
        direction = target.sub (origin).normalized ();
        plane_position = origin.add (direction);
        right = direction.cross (vec3 {0.0f, 0.0f, 1.0f}).normalized ();
        up = right.cross (direction).normalized ();
    }
};

struct Emitter
{
    vec3 color;
    float power;

    Emitter () : color (1.0f, 1.0f, 1.0f), power (1.0f) {}
    Emitter (const vec3& light_color, const float power_value) : color (light_color), power (power_value) {}
};

struct hit_info
{
    float distance = 1e6f;
    vec3 point;
    vec3 normal;
    vec3 color;
    const Emitter* emitter = nullptr;
};

struct ray
{
    vec3 origin;
    vec3 direction;
    vec3 pixel_color_contribution = {1.0f, 1.0f, 1.0f};
    hit_info info;
    int bounces = 0;

    ray () : origin (vec3 {}), direction (vec3 {1.0f, 0.0f, 0.0f}) {}
    ray (const vec3& origin_position, const vec3& direction_vector) : origin (origin_position), direction (direction_vector) {}
};

struct Object
{
    virtual ~Object () = default;
    virtual bool intersect (ray& ray) = 0;
};

struct sphere : Object
{
    vec3 origin;
    float radius;
    vec3 surface_color;

    sphere (const vec3& origin_position, const float radius_value, const vec3& surface_color_value) : origin (origin_position), radius (radius_value), surface_color (surface_color_value) {}

    vec3 normal (const vec3& point) const
    {
        vec3 P = point;
        vec3 O = origin;
        vec3 N = P.sub (O).normalized ();
        return N;
    }

    bool intersect (ray& ray)
    {
        vec3 O = ray.origin;
        vec3 C = origin;
        vec3 L = O.sub (C);
        vec3 D = ray.direction;
        float R = radius;

        float disc = (L.dot (D) * L.dot (D)) - (L.dot (L) - (R * R));

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
            vec3 I = O.add (D.mul (std::min (t1, t2)));
            float dist = I.sub (O).length ();

            if (dist < ray.info.distance)
            {
                ray.info.point = I;
                ray.info.distance = dist;
                ray.info.normal = normal (I);
                ray.info.color = surface_color;
                ray.info.emitter = nullptr;
                return true;
            }

        } else if ((t1 >= 0.0f) && (t2 < 0.0f))
        {
            vec3 I = O.add (D.mul (t1));
            float dist = I.sub (O).length ();

            if (dist < ray.info.distance)
            {
                ray.info.point = I;
                ray.info.distance = dist;
                ray.info.normal = normal (I);
                ray.info.color = surface_color;
                ray.info.emitter = nullptr;
                return true;
            }

        } else if ((t1 < 0.0f) && (t2 >= 0.0f))
        {
            vec3 I = O.add (D.mul (t2));
            float dist = I.sub (O).length ();

            if (dist < ray.info.distance)
            {
                ray.info.point = I;
                ray.info.distance = dist;
                ray.info.normal = normal (I);
                ray.info.color = surface_color;
                ray.info.emitter = nullptr;
                return true;
            }
        }

        return false;
    }
};

struct Plane : Object
{
    float z_level;
    float min_x;
    float max_x;
    float min_y;
    float max_y;
    vec3 normal;
    vec3 surface_color;
    std::optional <Emitter> emitter = std::nullopt;


    Plane () : z_level (0.0f), min_x (-1.0f), max_x (1.0f), min_y (-1.0f), max_y (1.0f), normal (0.0f, 0.0f, 1.0f), surface_color (1.0f, 1.0f, 1.0f) {}

    Plane (const float z_position, const float min_x_pos, const float max_x_pos, const float min_y_pos, const float max_y_pos, const vec3& normal_vec,
           const vec3& surface_color_value): z_level (z_position), min_x (min_x_pos), max_x (max_x_pos), min_y (min_y_pos), max_y (max_y_pos),
           normal (normal_vec), surface_color (surface_color_value) {}

    Plane (const float z_position, const float min_x_pos, const float max_x_pos, const float min_y_pos, const float max_y_pos, const vec3& normal_vec,
           const Emitter& emitter_info): z_level (z_position), min_x (min_x_pos), max_x (max_x_pos), min_y (min_y_pos), max_y (max_y_pos),
           normal (normal_vec), emitter (emitter_info) {}

    bool intersect (ray& ray)
    {
        float p_z = z_level;
        float ray_o_z = ray.origin.z;
        float ray_d_z = ray.direction.z;

        if (ray_d_z == 0.0f)
        {
            return false;
        }

        float t = (p_z - ray_o_z) / ray_d_z;

        if (t >= 0.0f)
        {
            vec3 I = ray.origin.add (ray.direction.mul (t));
            float dist = I.sub (ray.origin).length ();

            if (I.x > min_x && I.x < max_x && I.y > min_y && I.y < max_y && (dist < ray.info.distance))
            {
                ray.info.point = I;
                ray.info.distance = dist;
                ray.info.normal = normal;

                if (this -> emitter.has_value ())
                {
                    ray.info.emitter = &(*this -> emitter);

                } else
                {
                    ray.info.emitter = nullptr;
                    ray.info.color = surface_color;
                }

                return true;
            }
        }

        return false;
    }
};

inline vec3 calculate_pixel_cords (const camera3D& camera, const float pixel_x, const float pixel_y)
{
    float norm_px_x = pixel_x / camera.width;
    float norm_px_y = pixel_y / camera.height;
    vec3 cam_plane_left_up_corner_pos = camera.plane_position.add (-camera.right.mul (camera.plane_horizontal_size / 2.0f)).add (camera.up.mul (camera.plane_vertical_size / 2.0f));
    float rel_px_x_pos = (norm_px_x * camera.plane_horizontal_size) + (camera.plane_horizontal_size / (2 * camera.width));
    float rel_px_y_pos = (norm_px_y * camera.plane_vertical_size) + (camera.plane_vertical_size / (2 * camera.height));
    vec3 px_pos = cam_plane_left_up_corner_pos.add (camera.right.mul (rel_px_x_pos)).sub (camera.up.mul (rel_px_y_pos));
    return px_pos;
}

inline ray cast_ray (const camera3D& camera, const float x, const float y)
{
    vec3 ray_target = calculate_pixel_cords (camera, x, y);
    vec3 ray_dir = ray_target.sub (camera.origin).normalized ();
    ray ray = {camera.origin, ray_dir};
    return ray;
}

inline vec3 trace_ray (const std::vector <std::unique_ptr <Object>>& scene, ray& ray, const int max_bounces)
{
    vec3 sky_color = {};
    vec3 contrib;
    vec3 dir;

    while (ray.bounces < max_bounces)
    {
        bool hit_anything = false;
        contrib = ray.pixel_color_contribution;
        dir = ray.direction;

        for (const auto& obj : scene)
        {
            if (obj -> intersect (ray))
            {
                hit_anything = true;
            }
        }

        if (!hit_anything)
        {
            return contrib * sky_color;
        }

        if (ray.info.emitter != nullptr)
        {
            return contrib * (ray.info.emitter -> color).mul (ray.info.emitter -> power);
        }

        if (dir.dot (ray.info.normal) > 0.0f)
        {
            ray.info.normal = -ray.info.normal;
        }

        contrib *= ray.info.color;
        ray.origin = ray.info.point.add (ray.info.normal.mul (0.0001f));
        vec3 rand_vec = ray.info.normal.add (random_sphere_vector_normalized ()).normalized ();

        if (rand_vec.dot (ray.info.normal) < 0.0f)
        {
            rand_vec = -rand_vec;
        }

        dir = rand_vec;
        ray.direction = dir;
        ray.pixel_color_contribution = contrib;
        ray.bounces ++;
        ray.info.distance = 1e6f;
        ray.info.emitter = nullptr;
    }

    return {0.0f, 0.0f, 0.0f};
}

inline void init (const camera3D& camera)
{
    SetConfigFlags (FLAG_VSYNC_HINT);
    InitWindow ((int) camera.width, (int) camera.height, "Ray Casting 3D");
}

inline void exit ()
{
    CloseWindow ();
}
