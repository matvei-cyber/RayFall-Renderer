#pragma once
#include "math/vec/vec.hpp"
#include <random>


float random_float (std::mt19937& gen, std::uniform_real_distribution <float>& dist);
vec3 random_sphere_vector_normalized (std::mt19937& gen, std::uniform_real_distribution <float>& dist);
vec3 cosine_random_vector_nomralized (const vec3& normal, std::mt19937& gen, std::uniform_real_distribution <float>& dist);
