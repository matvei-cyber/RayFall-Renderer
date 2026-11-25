#pragma once
#include "ray/ray.hpp"
#include "camera/camera3D.hpp"
#include "geometry/Sphere.hpp"
#include <vector>


vec3 calculate_pixel_cords (const camera3D& camera, const float pixel_x, const float pixel_y);
ray cast_ray (const camera3D& camera, const float x, const float y);
vec3 trace_ray (const std::vector <std::unique_ptr <Object>>& scene, const vec3& env_color, ray& current_ray, const int max_bounces, std::mt19937& gen, std::uniform_real_distribution <float>& dist);
void init (const camera3D& camera);
void exit ();
