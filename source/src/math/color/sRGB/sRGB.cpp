#include "math/color/sRGB/sRGB.hpp"
#include <cmath>


vec3 linear_RGB_to_sRGB (const vec3& color)
{
    vec3 new_color;

    if (color.x <= 0.0031308f)
    {
        new_color.x = color.x * 12.92f;

    } else
    {
        new_color.x = (1.055f * std::powf (color.x, (1.0f / 2.4f))) - 0.055f;
    }

    if (color.y <= 0.0031308f)
    {
        new_color.y = color.y * 12.92f;

    } else
    {
        new_color.y = (1.055f * std::powf (color.y, (1.0f / 2.4f))) - 0.055f;
    }

    if (color.z <= 0.0031308f)
    {
        new_color.z = color.z * 12.92f;

    } else
    {
        new_color.z = (1.055f * std::powf (color.z, (1.0f / 2.4f))) - 0.055f;
    }

    return new_color;
}

void convert_linear_RGB_to_sRGB (vec3& color)
{
    if (color.x <= 0.0031308f)
    {
        color.x = color.x * 12.92f;

    } else
    {
        color.x = (1.055f * std::powf (color.x, (1.0f / 2.4f))) - 0.055f;
    }

    if (color.y <= 0.0031308f)
    {
        color.y = color.y * 12.92f;

    } else
    {
        color.y = (1.055f * std::powf (color.y, (1.0f / 2.4f))) - 0.055f;
    }

    if (color.z <= 0.0031308f)
    {
        color.z = color.z * 12.92f;

    } else
    {
        color.z = (1.055f * std::powf (color.z, (1.0f / 2.4f))) - 0.055f;
    }
}

void clamp_value (float& value, const float min_value, const float max_value)
{
    if (value < min_value)
    {
        value = min_value;

    } else if (value > max_value)
    {
        value = max_value;

    } else
    {
        return;
    }
}

void clamp_color (vec3& color, const float min_value, const float max_value)
{
    if (color.x < min_value)
    {
        color.x = min_value;

    } else if (color.x > max_value)
    {
        color.x = max_value;
    }


    if (color.y < min_value)
    {
        color.y = min_value;

    } else if (color.y > max_value)
    {
        color.y = max_value;
    }


    if (color.z < min_value)
    {
        color.z = min_value;

    } else if (color.z > max_value)
    {
        color.z = max_value;
    }
}
