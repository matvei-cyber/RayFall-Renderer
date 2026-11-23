#pragma once
#include "../math/vec/vec.hpp"


struct Emitter
{
    vec3 color;
    float power;


    Emitter ();
    Emitter (const vec3& light_color, const float power_value);
};
