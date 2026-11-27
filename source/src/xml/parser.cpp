#include "xml/parser.hpp"
#include "xml/pugixml.hpp"
#include "geometry/Sphere.hpp"
#include "geometry/Plane.hpp"
#include "material/Diffuse.hpp"
#include "math/vec/vec.hpp"
#include <iostream>

std::vector <Object*> parse_scene_from_xml ()
{
	std::vector <Object*> scene;

	pugi::xml_document doc;
	if (!doc.load_file ("scene.xml"))
	{
		std::cerr << "XML PARSER ERROR: unable to load/read scene.xml.\n";
		return scene;
	}

	pugi::xml_node scene_block = doc.child ("scene");
	if (!scene_block)
	{
		std::cerr << "XML PARSER ERROR: <scene> root block not found.\n";
		return scene;
	}

	pugi::xml_node camera_block = scene_block.child ("camera");
	if (!camera_block)
	{
		std::cerr << "XML PARSER ERROR: <camera> block not found.\n";
		return scene;
	}

	pugi::xml_node geometry_block = scene_block.child ("geometry");
	if (!geometry_block)
	{
		std::cerr << "XML PARSER ERROR: <geometry> block not found.\n";
		return scene;
	}

	for (pugi::xml_node obj_node : geometry_block.children ("object"))
	{
		std::string type = obj_node.attribute ("type").as_string ();

		if (type == "")
		{
			std::cerr << "XML PARSER WARNING: some objects have invalid type.\n";

		} else if (type == "sphere")
		{
			Sphere* sphere = new Sphere ();

			pugi::xml_node orig_node = obj_node.child ("origin");
			if (!orig_node)
			{
				std::cerr << "XML PARSER WARNING: some objects with <sphere> type have no origin parameter. Setting default: x = 0, y = 0, z = 0.\n";
			}

			vec3 sphere_origin;
			sphere_origin.x = orig_node.attribute ("x").as_float (0.0f);
			sphere_origin.y = orig_node.attribute ("y").as_float (0.0f);
			sphere_origin.z = orig_node.attribute ("z").as_float (0.0f);
			sphere -> set_origin (sphere_origin);

			pugi::xml_node sphere_radius = obj_node.child ("radius");
			if (!sphere_radius)
			{
				std::cerr << "XML PARSER WARNING: some objects with <sphere> type have no radius parameter. Setting default: 1.\n";
			}

			sphere -> set_radius (sphere_radius.text ().as_float (1.0f));

			pugi::xml_node bsdf_node = obj_node.child ("BSDF");
			pugi::xml_node emit_node = obj_node.child ("emitter");

			if (bsdf_node && emit_node)
			{
				std::cerr << "XML PARSER ERROR: some objects have <BSDF> and <emitter> blocks at the same time.\n";
				return scene;

			} else if (bsdf_node && !emit_node)
			{
				if (std::string (bsdf_node.attribute ("type").as_string ("diffuse")) == "diffuse")
				{
					BSDF* diff = new Diffuse ();

					pugi::xml_node col_node = bsdf_node.child ("color");
					if (!col_node)
					{
						std::cerr << "XML PARSER WARNING: some objects with <diffuse> material have no color. Setting default: r = 0.85, g = 0.85, b = 0.85.\n";
					}

					diff -> albedo.x = col_node.attribute ("r").as_float (0.85f);
					diff -> albedo.y = col_node.attribute ("g").as_float (0.85f);
					diff -> albedo.z = col_node.attribute ("b").as_float (0.85f);
					sphere -> set_BSDF (diff);
				}
			} else if (emit_node && !bsdf_node)
			{
				Emitter* emit = new Emitter ();

				pugi::xml_node col_node = emit_node.child ("color");
				if (!col_node)
				{
					std::cerr << "XML PARSER WARNING: some emitting objects with <sphere> type have no <color> parameter. Setting default: r = 1, g = 1, b = 1.\n";
				}

				pugi::xml_node pow_node = emit_node.child ("power");
				if (!pow_node)
				{
					std::cerr << "XML PARSER WARNING: some emitting objects with <sphere> type have no <power> parameter. Setting default: 1.\n";
				}

				emit -> color.x = col_node.attribute ("r").as_float (1.0f);
				emit -> color.y = col_node.attribute ("g").as_float (1.0f);
				emit -> color.z = col_node.attribute ("b").as_float (1.0f);
				emit -> power = pow_node.text ().as_float (1.0f);
				sphere -> set_emitter (emit);
			}

			scene.push_back (sphere);

		} else if (type == "plane")
		{
			Plane* plane = new Plane ();

			pugi::xml_node z_pos_node = obj_node.child ("z_position");
			if (!z_pos_node)
			{
				std::cerr << "XML PARSER WARNING: some objects with <plane> type have no <z_position> parameter. Setting default: 0.\n";
			}

			float z_pos = z_pos_node.text ().as_float (0.0f);
			plane -> set_z_level (z_pos);

			pugi::xml_node min_x_node = obj_node.child ("min_x");
			if (!min_x_node)
			{
				std::cerr << "XML PARSER WARNING: some objects with <plane> type have no <min_x> parameter. Setting default: -1.\n";
			}

			float min_x = min_x_node.text ().as_float (-1.0f);
			plane -> set_min_x (min_x);

			pugi::xml_node max_x_node = obj_node.child ("max_x");
			if (!max_x_node)
			{
				std::cerr << "XML PARSER WARNING: some objects with <plane> type have no <max_x> parameter. Setting default: 1.\n";
			}

			float max_x = max_x_node.text ().as_float (1.0f);
			plane -> set_max_x (max_x);

			pugi::xml_node min_y_node = obj_node.child ("min_y");
			if (!min_y_node)
			{
				std::cerr << "XML PARSER WARNING: some objects with <plane> type have no <min_y> parameter. Setting default: -1.\n";
			}

			float min_y = min_y_node.text ().as_float (-1.0f);
			plane -> set_min_y (min_y);

			pugi::xml_node max_y_node = obj_node.child ("max_y");
			if (!max_y_node)
			{
				std::cerr << "XML PARSER WARNING: some objects with <plane> type have no <max_y> parameter. Setting default: 1.\n";
			}

			float max_y = max_y_node.text ().as_float (1.0f);
			plane -> set_max_y (max_y);

			pugi::xml_node normal_node = obj_node.child ("normal");
			if (!normal_node)
			{
				std::cerr << "XML PARSER WARNING: some objects with <plane> type have no <normal> parameter. Setting default: x = 0, y = 0, z = 1.\n";
			}

			vec3 normal = {};
			normal.x = normal_node.attribute ("x").as_float (0.0f);
			normal.y = normal_node.attribute ("y").as_float (0.0f);
			normal.z = normal_node.attribute ("z").as_float (1.0f);
			plane -> set_normal (normal);

			pugi::xml_node bsdf_node = obj_node.child ("BSDF");
			pugi::xml_node emit_node = obj_node.child ("emitter");

			if (bsdf_node && emit_node)
			{
				std::cerr << "XML PARSER ERROR: some objects have <BSDF> and <emitter> blocks at the same time.\n";
				return scene;

			} else if (bsdf_node && !emit_node)
			{
				if (std::string (bsdf_node.attribute ("type").as_string ("diffuse")) == "diffuse")
				{
					BSDF* diff = new Diffuse ();

					pugi::xml_node col_node = bsdf_node.child ("color");
					if (!col_node)
					{
						std::cerr << "XML PARSER WARNING: some objects with <diffuse> material have no color. Setting default: r = 0.85, g = 0.85, b = 0.85.\n";
					}

					diff -> albedo.x = col_node.attribute ("r").as_float (0.85f);
					diff -> albedo.y = col_node.attribute ("g").as_float (0.85f);
					diff -> albedo.z = col_node.attribute ("b").as_float (0.85f);
					plane -> set_material (diff);
				}
			} else if (emit_node && !bsdf_node)
			{
				Emitter* emit = new Emitter ();

				pugi::xml_node col_node = emit_node.child ("color");
				if (!col_node)
				{
					std::cerr << "XML PARSER WARNING: some emitting objects with <sphere> type have no <color> parameter. Setting default: r = 1, g = 1, b = 1.\n";
				}

				pugi::xml_node pow_node = emit_node.child ("power");
				if (!pow_node)
				{
					std::cerr << "XML PARSER WARNING: some emitting objects with <sphere> type have no <power> parameter. Setting default: 1.\n";
				}

				emit -> color.x = col_node.attribute ("r").as_float (1.0f);
				emit -> color.y = col_node.attribute ("g").as_float (1.0f);
				emit -> color.z = col_node.attribute ("b").as_float (1.0f);
				emit -> power = pow_node.text ().as_float (1.0f);
				plane -> set_emitter (emit);
			}

			scene.push_back (plane);
		}
	}

	return scene;
}

camera3D parse_camera_from_xml ()
{
	pugi::xml_document doc;
	if (!doc.load_file ("scene.xml"))
	{
		std::cerr << "XML PARSER ERROR: unable to load/read scene.xml.\n";
	}

	pugi::xml_node scene_block = doc.child ("scene");
	if (!scene_block)
	{
		std::cerr << "XML PARSER ERROR: unable to load/read scene.xml.\n";
	}

	pugi::xml_node camera_block = scene_block.child ("camera");
	if (!camera_block)
	{
		std::cerr << "XML PARSER ERROR: <camera> block not found.\n";
	}

	camera3D camera = {};

	pugi::xml_node orig_node = camera_block.child ("origin");
	if (!orig_node)
	{
		std::cerr << "XML PARSER WARNING: camera has no origin parameter.\n";
	}

	vec3 camera_origin = {};
	camera_origin.x = orig_node.attribute ("x").as_float (1.0f);
	camera_origin.y = orig_node.attribute ("y").as_float (1.0f);
	camera_origin.z = orig_node.attribute ("z").as_float (1.0f);
	camera.set_origin (camera_origin);

	pugi::xml_node target_node = camera_block.child ("look_at");
	if (!target_node)
	{
		std::cerr << "XML PARSER WARNING: camera has no <look_at> parameter. Setting default: x = 0, y = 0, z = 0.\n";
	}

	vec3 camera_target = {};
	camera_target.x = target_node.attribute ("x").as_float (0.0f);
	camera_target.y = target_node.attribute ("y").as_float (0.0f);
	camera_target.z = target_node.attribute ("z").as_float (0.0f);
	camera.set_target (camera_target);

	pugi::xml_node size_node = camera_block.child ("size");
	if (!size_node)
	{
		std::cerr << "XML PARSER WARNING: camera has no <size> parameter. Setting default: x = 1920, y = 1080.\n";
	}

	int camera_width;
	int camera_height;
	camera_width = size_node.attribute ("x").as_int (1920);
	camera_height = size_node.attribute ("y").as_int (1080);
	camera.set_size (camera_width, camera_height);

	pugi::xml_node fov_node = camera_block.child ("FOV");
	if (!fov_node)
	{
		std::cerr << "XML PARSER WARNING: camera hasn't an <FOV> parameter. Setting default: 90 degrees.\n";
	}

	float camera_fov = fov_node.text ().as_float (90.0f);
	camera.set_FOV_degrees (camera_fov);

	return camera;
}
