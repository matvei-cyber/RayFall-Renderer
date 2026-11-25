#include "math/vec/vec.hpp"
#include <cmath>


vec2::vec2 () : x (0.0f), y (0.0f) {}
vec2::vec2 (const float x_val, const float y_val) : x (x_val), y (y_val) {}


vec2 vec2::add (const vec2& other) const
{
    return vec2 {x + other.x, y + other.y};
}

void vec2::add_rw (const vec2& other)
{
    x += other.x;
    y += other.y;
}

vec2 vec2::sub (const vec2& other) const
{
    return vec2 {x - other.x, y - other.y};
}

void vec2::sub_rw (const vec2& other)
{
    x -= other.x;
    y -= other.y;
}

vec2 vec2::operator- () const
{
    return vec2 {-x, -y};
}

vec2 vec2::mul (const float num) const
{
    return vec2 {x * num, y * num};
}

void vec2::mul_rw (const float num)
{
    x *= num;
    y *= num;
}

vec2 vec2::div (const float num) const
{
    if (num == 0.0f)
    {
        return *this;

    } else
    {
        return vec2 {x / num, y / num};
    }
}

void vec2::div_rw (const float num)
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

vec2 vec2::operator* (const vec2& other) const
{
    return vec2 {x * other.x, y * other.y};
}

void vec2::operator*= (const vec2& other)
{
    x *= other.x;
    y *= other.y;
}

bool vec2::operator== (const vec2& other) const
{
    if ((x == other.x) && (y == other.y))
    {
        return true;

    } else
    {
        return false;
    }
}

float vec2::dot (const vec2& other) const
{
    return (x * other.x) + (y * other.y);
}

float vec2::length () const
{
    return std::sqrtf (this -> dot (*this));
}

void vec2::normalize ()
{
    float len = length ();

    if (len == 0.0f)
    {
        return;

    } else
    {
        return this -> div_rw (len);
    }
}

vec2 vec2::normalized () const
{
    float len = length ();

    if (len == 0.0f)
    {
        return *this;

    } else
    {
        return this -> div (len);
    }
}

vec2 vec2::right_normal () const
{
    return vec2 {y, -x};
}

vec2 vec2::left_normal () const
{
    return vec2 {-y, x};
}




vec3::vec3 () : x (0.0f), y (0.0f), z (0.0f) {}
vec3::vec3 (const float x_val, const float y_val, const float z_val) : x (x_val), y (y_val), z (z_val) {}


vec3 vec3::add (const vec3& other) const
{
    return vec3 {x + other.x, y + other.y, z + other.z};
}

void vec3::add_rw (const vec3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}

vec3 vec3::sub (const vec3& other) const
{
    return vec3 {x - other.x, y - other.y, z - other.z};
}

void vec3::sub_rw (const vec3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

vec3 vec3::operator- () const
{
    return vec3 {-x, -y, -z};
}

vec3 vec3::mul (const float num) const
{
    return vec3 {x * num, y * num, z * num};
}

void vec3::mul_rw (const float num)
{
    x *= num;
    y *= num;
    z *= num;
}

vec3 vec3::div (const float num) const
{
    if (num == 0.0f)
    {
        return *this;

    } else
    {
        return vec3 {x / num, y / num, z / num};
    }
}

void vec3::div_rw (const float num)
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

vec3 vec3::operator* (const vec3& other) const
{
    return vec3 {x * other.x, y * other.y, z * other.z};
}

void vec3::operator*= (const vec3& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
}

bool vec3::operator== (const vec3& other) const
{
    if ((x == other.x) && (y == other.y) && (z == other.z))
    {
        return true;

    } else
    {
        return false;
    }
}

float vec3::dot (const vec3& other) const
{
    return (x * other.x) + (y * other.y) + (z * other.z);
}

vec3 vec3::cross (const vec3& other) const
{
    vec3 res;
    res.x = (y * other.z) - (z * other.y);
    res.y = (z * other.x) - (x * other.z);
    res.z = (x * other.y) - (y * other.x);
    return res;
}

float vec3::length () const
{
    return std::sqrtf (this -> dot (*this));
}

void vec3::normalize ()
{
    float len = length ();

    if (len == 0.0f)
    {
        return;

    } else
    {
        return this -> div_rw (len);
    }
}

vec3 vec3::normalized () const
{
    float len = length ();

    if (len == 0.0f)
    {
        return *this;

    } else
    {
        return this -> div (len);
    }
}
