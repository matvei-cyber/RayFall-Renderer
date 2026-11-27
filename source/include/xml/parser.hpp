#pragma once
#include "../geometry/Object.hpp"
#include "../camera/camera3D.hpp"
#include <vector>


std::vector <Object*> parse_scene_from_xml ();
camera3D parse_camera_from_xml ();
