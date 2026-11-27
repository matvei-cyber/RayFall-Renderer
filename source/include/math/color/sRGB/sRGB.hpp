#pragma once
#include "../../vec/vec.hpp"


vec3 linear_RGB_to_sRGB (const vec3& color);
void convert_linear_RGB_to_sRGB (vec3& color);
void clamp_value (float& value, const float min_value, const float max_value);
void clamp_color (vec3& color, const float min_value, const float max_value);
