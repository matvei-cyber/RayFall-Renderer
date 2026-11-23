#pragma once
#include "../math/vec/vec.hpp"


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
    int width;
    int height;
    vec3 plane_position;
    float plane_horizontal_size;
    float plane_vertical_size;
    vec3 plane_left_up_corner_pos;


    camera3D ();
    camera3D (const vec3& origin_position, const vec3& target_position, const float horizontal_FOV_in_degrees, const int screen_width, const int screen_height);
};
