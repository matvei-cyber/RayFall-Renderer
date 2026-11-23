#include "math/angle.hpp"
#include "math/consts.hpp"


float deg_to_rad (const float deg)
{
    return (deg * pi) / 180.0f;
}

float rad_to_deg (const float rad)
{
    return (rad / pi) * 180.0f;
}
