#include "camera/camera3D.hpp"
#include "math/angle.hpp"
#include <cmath>


camera3D::camera3D () : origin (vec3 {}), target (1.0f, 0.0f, 0.0f), horizontal_FOV_degrees (90.0f), width (1920), height (1080)
{
    horizontal_FOV = deg_to_rad (horizontal_FOV_degrees);
    aspect_ratio = static_cast <float> (width) / static_cast <float> (height);
    plane_horizontal_size = 2 * std::tanf (horizontal_FOV / 2.0f);
    plane_vertical_size = plane_horizontal_size / aspect_ratio;
    direction = target.sub (origin).normalized ();
    plane_position = origin.add (direction);
    right = direction.cross (vec3 {0.0f, 0.0f, 1.0f}).normalized ();
    up = right.cross (direction).normalized ();
    plane_left_up_corner_pos = plane_position.add (-right.mul (plane_horizontal_size / 2.0f)).add (up.mul (plane_vertical_size / 2.0f));
}

camera3D::camera3D (const vec3& origin_position, const vec3& target_position, const float horizontal_FOV_in_degrees, const int screen_width, const int screen_height):
origin (origin_position), target (target_position), horizontal_FOV_degrees (horizontal_FOV_in_degrees), width (screen_width), height (screen_height)
{
    horizontal_FOV = deg_to_rad (horizontal_FOV_degrees);
    aspect_ratio = static_cast <float> (width) / static_cast <float> (height);
    plane_horizontal_size = 2 * std::tanf (horizontal_FOV / 2.0f);
    plane_vertical_size = plane_horizontal_size / aspect_ratio;
    direction = target.sub (origin).normalized ();
    plane_position = origin.add (direction);
    right = direction.cross (vec3 {0.0f, 0.0f, 1.0f}).normalized ();
    up = right.cross (direction).normalized ();
    plane_left_up_corner_pos = plane_position.add (-right.mul (plane_horizontal_size / 2.0f)).add (up.mul (plane_vertical_size / 2.0f));
}
