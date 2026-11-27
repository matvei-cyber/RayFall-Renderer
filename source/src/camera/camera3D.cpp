#include "camera/camera3D.hpp"
#include "math/angle.hpp"
#include <cmath>


camera3D::camera3D () : origin (vec3 {}), target (1.0f, 0.0f, 0.0f), horizontal_FOV_degrees (90.0f), width (1920), height (1080)
{
    horizontal_FOV = deg_to_rad (horizontal_FOV_degrees);
    aspect_ratio = static_cast <float> (width) / static_cast <float> (height);
    plane_size.x = 2 * std::tanf (horizontal_FOV / 2.0f);
    plane_size.y = plane_size.x / aspect_ratio;
    direction = target.sub (origin).normalized ();
    plane_position = origin.add (direction);
    right = direction.cross (vec3 {0.0f, 0.0f, 1.0f}).normalized ();
    up = right.cross (direction).normalized ();
    plane_left_up_corner_pos = plane_position.add (-right.mul (plane_size.x / 2.0f)).add (up.mul (plane_size.y / 2.0f));
}

camera3D::camera3D (const vec3& origin_position, const vec3& target_position, const float horizontal_FOV_in_degrees, const int screen_width, const int screen_height):
origin (origin_position), target (target_position), horizontal_FOV_degrees (horizontal_FOV_in_degrees), width (screen_width), height (screen_height)
{
    horizontal_FOV = deg_to_rad (horizontal_FOV_degrees);
    aspect_ratio = static_cast <float> (width) / static_cast <float> (height);
    plane_size.x = 2 * std::tanf (horizontal_FOV / 2.0f);
    plane_size.y = plane_size.x / aspect_ratio;
    direction = target.sub (origin).normalized ();
    plane_position = origin.add (direction);
    right = direction.cross (vec3 {0.0f, 0.0f, 1.0f}).normalized ();
    up = right.cross (direction).normalized ();
    plane_left_up_corner_pos = plane_position.add (-right.mul (plane_size.x / 2.0f)).add (up.mul (plane_size.y / 2.0f));
}

vec2 camera3D::get_size () const
{
    return vec2 {static_cast <float> (width), static_cast <float> (height)};
}

vec2 camera3D::get_plane_size () const
{
    return plane_size;
}

vec3 camera3D::get_right () const
{
    return right;
}

vec3 camera3D::get_up () const
{
    return up;
}

vec3 camera3D::get_origin () const
{
    return origin;
}

vec3 camera3D::get_plane_corner () const
{
    return plane_left_up_corner_pos;
}

void camera3D::set_origin (const vec3& new_origin)
{
    origin = new_origin;
    direction = target.sub (origin).normalized ();
    plane_position = origin.add (direction);
    right = direction.cross (vec3 {0.0f, 0.0f, 1.0f}).normalized ();
    up = right.cross (direction).normalized ();
    plane_left_up_corner_pos = plane_position.add (-right.mul (plane_size.x / 2.0f)).add (up.mul (plane_size.y / 2.0f));
}

void camera3D::set_target (const vec3& new_target)
{
    if (new_target != origin)
    {
        target = new_target;
        direction = target.sub (origin).normalized ();
        plane_position = origin.add (direction);
        right = direction.cross (vec3 {0.0f, 0.0f, 1.0f}).normalized ();
        up = right.cross (direction).normalized ();
        plane_left_up_corner_pos = plane_position.add (-right.mul (plane_size.x / 2.0f)).add (up.mul (plane_size.y / 2.0f));
    }
}

void camera3D::set_FOV_degrees (const float new_FOV_degrees)
{
    if ((new_FOV_degrees < 180.0f) && (new_FOV_degrees > 0.0f))
    {
        horizontal_FOV = deg_to_rad (new_FOV_degrees);
        plane_size.x = 2 * std::tanf (horizontal_FOV / 2.0f);
        plane_size.y = plane_size.x / aspect_ratio;
        plane_left_up_corner_pos = plane_position.add (-right.mul (plane_size.x / 2.0f)).add (up.mul (plane_size.y / 2.0f));
    }
}

void camera3D::set_size (const int new_width, const int new_height)
{
    if ((new_width > 0) && (new_height > 0))
    {
        aspect_ratio = static_cast <float> (width) / static_cast <float> (height);
        plane_size.x = 2 * std::tanf (horizontal_FOV / 2.0f);
        plane_size.y = plane_size.x / aspect_ratio;
        plane_left_up_corner_pos = plane_position.add (-right.mul (plane_size.x / 2.0f)).add (up.mul (plane_size.y / 2.0f));
    }
}
