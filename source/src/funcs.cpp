#include "funcs.hpp"
#include "raylib/raylib.h"
#include "geometry/Sphere.hpp"
#include "math/consts.hpp"
#include "math/random/random.hpp"


vec3 calculate_pixel_cords (const camera3D& camera, const float pixel_x, const float pixel_y)
{
    vec2 camera_size = camera.get_size ();
    vec2 camera_plane = camera.get_plane_size ();
    vec3 camera_right = camera.get_right ();
    vec3 camera_up = camera.get_up ();
    vec3 camera_plane_corner = camera.get_plane_corner ();

    float norm_px_x = pixel_x / static_cast <float> (camera_size.x);
    float norm_px_y = pixel_y / static_cast <float> (camera_size.y);

    float rel_px_x_pos = (norm_px_x * camera_plane.x) + (camera_plane.x / (2 * camera_size.x));
    float rel_px_y_pos = (norm_px_y * camera_plane.y) + (camera_plane.y / (2 * camera_size.y));

    vec3 px_pos = camera_plane_corner.add (camera_right.mul (rel_px_x_pos)).sub (camera_up.mul (rel_px_y_pos));
    return px_pos;
}

ray cast_ray (const camera3D& camera, const float x, const float y)
{
    vec3 camera_origin = camera.get_origin ();
    vec3 ray_target = calculate_pixel_cords (camera, x, y);
    vec3 ray_dir = ray_target.sub (camera_origin).normalized ();

    ray r = {camera_origin, ray_dir};
    return r;
}

vec3 trace_ray (const std::vector <Object*>& scene, const vec3& env_color, ray& current_ray, const int max_bounces, std::mt19937& gen, std::uniform_real_distribution <float>& dist)
{
    bool is_hit;
    int bounces = current_ray.get_bounces ();
    vec3 contrib;

    while (bounces < max_bounces)
    {
        is_hit = false;
        contrib = current_ray.get_pixel_color_contribution ();
        bounces = current_ray.get_bounces ();

        for (const auto& obj : scene)
        {
            if (obj -> intersect (current_ray))
            {
                is_hit = true;
            }
        }

        if (!is_hit)
        {
            return env_color * contrib;
        }

        if (current_ray.info.emitter != nullptr)
        {
            vec3 radiance = current_ray.info.emitter -> color.mul (current_ray.info.emitter -> power);
            return contrib * radiance;
        }

        current_ray.info.material -> scatter (current_ray, gen, dist);
    }

    return vec3 {};
}

void main_path_tracing (const camera3D& camera, const float x, const float y, const std::vector <Object*>& scene, const int max_bounces,
                        std::mt19937& gen, std::uniform_real_distribution <float>& dist, std::vector <vec3>& accum_buffer)
{
    vec2 camera_size = camera.get_size ();
    ray r = cast_ray (camera, static_cast <float> (x), static_cast <float> (y));
    vec3 color = trace_ray (scene, vec3 {}, r, max_bounces, gen, dist);
    accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera_size.x) + x)].add_rw (color);
}

void init (const camera3D& camera)
{
    vec2 camera_size = camera.get_size ();

    SetConfigFlags (FLAG_VSYNC_HINT);
    InitWindow (camera_size.x, camera_size.y, "Ray Casting 3D");
}

void exit ()
{
    CloseWindow ();
}
