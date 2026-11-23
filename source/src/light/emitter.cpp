#include "light/emitter.hpp"


Emitter::Emitter () : color (1.0f, 1.0f, 1.0f), power (1.0f) {}
Emitter::Emitter (const vec3& light_color, const float power_value) : color (light_color), power (power_value) {}
