#include "math/random/random.hpp"
#include "math/consts.hpp"


float random_float (std::mt19937& gen, std::uniform_real_distribution <float>& dist)
{
    return dist (gen);
}

vec3 random_sphere_vector_normalized (std::mt19937& gen, std::uniform_real_distribution <float>& dist)
{
    vec3 rand_vec = {};

    while (true)
    {
        rand_vec.x = random_float (gen, dist);
        rand_vec.y = random_float (gen, dist);
        rand_vec.z = random_float (gen, dist);

        if (rand_vec.dot (rand_vec) <= 1.0f)
        {
            return rand_vec.normalized ();
        }
    }
}

vec3 cosine_random_vector_nomralized (const vec3& normal, std::mt19937& gen, std::uniform_real_distribution <float>& dist)
{
    vec3 rand_vec_uniform = random_sphere_vector_normalized (gen, dist);
    vec3 rand_normal_sum = rand_vec_uniform.add (normal);

    if (rand_normal_sum.dot (rand_normal_sum) < 0.000001f)
    {
        return normal;
    }

    return rand_normal_sum.normalized ();
}
