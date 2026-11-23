#include "funcs.hpp"
#include "raylib/raylib.h"


vec3 calculate_pixel_cords (const camera3D& camera, const float pixel_x, const float pixel_y)
{
    float norm_px_x = pixel_x / static_cast <float> (camera.width);
    float norm_px_y = pixel_y / static_cast <float> (camera.height);
    float rel_px_x_pos = (norm_px_x * camera.plane_horizontal_size) + (camera.plane_horizontal_size / (2 * static_cast <float> (camera.width)));
    float rel_px_y_pos = (norm_px_y * camera.plane_vertical_size) + (camera.plane_vertical_size / (2 * static_cast <float> (camera.height)));
    vec3 px_pos = camera.plane_left_up_corner_pos.add (camera.right.mul (rel_px_x_pos)).sub (camera.up.mul (rel_px_y_pos));
    return px_pos;
}

ray cast_ray_from_camera (const camera3D& camera, const float x, const float y)
{
    vec3 ray_target = calculate_pixel_cords (camera, x, y);
    vec3 ray_dir = ray_target.sub (camera.origin).normalized ();
    ray ray = {camera.origin, ray_dir};
    return ray;
}

vec3 trace_ray (const std::vector <std::unique_ptr <Object>>& scene, ray& current_ray, const int max_bounces)
{
    // vec3 sky_color = {0.5f, 0.8f, 1.0f};
    vec3 sky_color = {};
    vec3 contrib;

    while (current_ray.bounces < max_bounces)
    {
        bool hit_anything = false;
        contrib = current_ray.pixel_color_contribution;

        for (const auto& obj : scene)
        {
            if (obj -> intersect (current_ray))
            {
                hit_anything = true;
            }
        }

        if (!hit_anything)
        {
            return contrib * sky_color;
        }

        if (current_ray.info.emitter != nullptr)
        {
            return contrib * (current_ray.info.emitter -> color).mul (current_ray.info.emitter -> power);
        }

        if (current_ray.direction.dot (current_ray.info.normal) > 0.0f)
        {
            current_ray.info.normal = -current_ray.info.normal;
        }

        current_ray.info.material -> scatter (current_ray);
    }

    return vec3 {};
}

void init (const camera3D& camera)
{
    SetConfigFlags (FLAG_VSYNC_HINT);
    InitWindow (camera.width, camera.height, "Ray Casting 3D");
}

void exit ()
{
    CloseWindow ();
}
