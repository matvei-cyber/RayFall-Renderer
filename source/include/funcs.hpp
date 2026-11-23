#pragma once
#include "math/vec/vec.hpp"
#include "ray/ray.hpp"
#include "camera/camera3D.hpp"
#include "geometry/Object.hpp"
#include <vector>
#include <memory>


vec3 calculate_pixel_cords (const camera3D& camera, const float pixel_x, const float pixel_y);
ray cast_ray_from_camera (const camera3D& camera, const float x, const float y);
vec3 trace_ray (const std::vector <std::unique_ptr <Object>>& scene, ray& current_ray, const int max_bounces);
void init (const camera3D& camera);
void exit ();
