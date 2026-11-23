#include "math/random/random.hpp"
#include "math/consts.hpp"
#include "sodium.h"
// SODIUM MUST BE INITIALIZED IN MAIN.CPP BY WRITING SODIUM_INIT () COMMAND !!!!


float random_float_normalized ()
{
    uint32_t rand_num = randombytes_random ();
    return static_cast <float> (rand_num >> 8) * 0x1.0p-24f;
}

vec3 random_sphere_vector_normalized ()
{
    float phi = 2.0f * pi * random_float_normalized ();
    float z = (2.0f * random_float_normalized ()) - 1.0f;
    float r = std::sqrtf (1.0f - (z * z));
    vec3 rand_vec = {r * std::cosf (phi), r * std::sinf (phi), z};
    return rand_vec.normalized (); // I NEED TO ADD NORMAL VECTOR TO THIS IN TRACE_RAY () AND NORMALIZE THIS RESULT !!!!
}
