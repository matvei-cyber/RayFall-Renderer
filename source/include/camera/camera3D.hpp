#pragma once
#include "../math/vec/vec.hpp"


struct camera3D
{
    private:

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
        vec2 plane_size;
        vec3 plane_left_up_corner_pos;

    public:

        camera3D ();
        camera3D (const vec3& origin_position, const vec3& target_position, const float horizontal_FOV_in_degrees, const int screen_width, const int screen_height);

        vec2 get_size () const;
        vec2 get_plane_size () const;
        vec3 get_right () const;
        vec3 get_up () const;
        vec3 get_origin () const;
        vec3 get_plane_corner () const;
};
