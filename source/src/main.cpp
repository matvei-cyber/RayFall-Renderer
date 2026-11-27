#include "camera/camera3D.hpp"
#include "geometry/Object.hpp"
#include "geometry/Plane.hpp"
#include "geometry/Sphere.hpp"
#include "light/emitter.hpp"
#include "material/Diffuse.hpp"
#include "material/Glossy.hpp"
#include "math/color/sRGB/sRGB.hpp"
#include "math/random/random.hpp"
#include "math/vec/vec.hpp"
#include "ray/ray.hpp"
#include "raylib/raylib.h"
#include "funcs.hpp"
#include "xml/parser.hpp"
#include <iostream>
#include <random>

int main ()
{
    std::vector <Object*> scene = parse_scene_from_xml ();
    camera3D camera = parse_camera_from_xml ();

    vec2 camera_size = camera.get_size ();
    int pixel_count = camera_size.x * camera_size.y;

    std::mt19937 gen (std::random_device {}());
    std::uniform_real_distribution <float> dist (-1.0f, 1.0f);

    //vec2 camera_size = {1920.0f, 1080.0f};
    //camera3D camera = {vec3 {0.75f, -0.4f, 0.5f}, vec3 {-2.0f, 0.5f, 0.0f}, 90.0f, static_cast <int> (camera_size.x), static_cast <int> (camera_size.y)};

    //std::vector <std::unique_ptr <Object>> scene;
    //scene.emplace_back (std::make_unique <Plane> (0.0f, -20.5f, 20.5f, -20.5f, 20.5f, vec3 {0.0f, 0.0f, 1.0f}, std::make_shared <Diffuse> (vec3 {0.85f, 0.85f, 0.85f})));
    //scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.5f, 0.5f, 0.2501f}, 0.25f, std::make_shared <Glossy> (vec3 {0.96f, 0.96f, 0.96f}, 0.0f)));
    //scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.5f, -0.5f, 0.2501f}, 0.25f, std::make_shared <Diffuse> (vec3 {0.0f, 0.85f, 0.3f})));
    //scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.1f, 0.3f, 0.1251f}, 0.125f, std::make_shared <Diffuse> (vec3 {0.2f, 0.0f, 0.85f})));
    //scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.12f, -0.15f, 0.4f}, 0.125f, std::make_shared <Diffuse> (vec3 {0.3f, 0.5f, 0.85f})));
    //scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.02f, -0.1f, 0.0501f}, 0.05f, std::make_shared <Diffuse> (vec3 {0.1f, 0.1f, 0.85f})));
    // 
    //Sphere lamp = {vec3 {1.0f, 1.0f, 2.0f}, 1.0f, Emitter {vec3 {1.0f, 1.0f, 1.0f}, 2.5f}};
    //scene.emplace_back (std::make_unique <Sphere> (lamp));

    int max_bounces = 2;
    int max_samples = 128;

    std::vector <Color> pixels (static_cast <std::vector <Color>::size_type> (pixel_count), BLACK);
    Image img = GenImageColor (camera_size.x, camera_size.y, BLACK);
    init (camera);
    Texture2D texture = LoadTextureFromImage (img);
    UnloadImage (img);
    std::vector <vec3> accum_buffer (static_cast <std::vector <Color>::size_type> (pixel_count), vec3 {});

    for (int samples_done = 0; samples_done < max_samples; samples_done++)
    {
        for (int y = 0; y < camera_size.y; y++)
        {
            for (int x = 0; x < camera_size.x; x++)
            {
                ray r = cast_ray (camera, static_cast <float> (x), static_cast <float> (y));
                vec3 color = trace_ray (scene, vec3 {}, r, max_bounces, gen, dist);
                accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera_size.x) + x)].add_rw (color);
            }
        }

        std::cout << std::endl << "SPP: " << samples_done;
    }

    std::cout << std::endl << "SPP: " << max_samples << std::endl;

    for (int y = 0; y < camera_size.y; y++)
    {
        for (int x = 0; x < camera_size.x; x++)
        {
            accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera_size.x) + x)].div_rw (static_cast <float> (max_samples));
            clamp_color (accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera_size.x) + x)], 0.0f, 1.0f);
            convert_linear_RGB_to_sRGB (accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera_size.x) + x)]);

            pixels [static_cast <std::vector <Color>::size_type> ((y * camera_size.x) + x)] = Color
            {
                static_cast <unsigned char> (accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera_size.x) + x)].x * 255),
                static_cast <unsigned char> (accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera_size.x) + x)].y * 255),
                static_cast <unsigned char> (accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera_size.x) + x)].z * 255),
                255
            };
        }
    }

    UpdateTexture (texture, pixels.data ());

    while (!WindowShouldClose ())
    {
        BeginDrawing ();
            ClearBackground (BLACK);
            DrawTexture (texture, 0, 0, WHITE);
        EndDrawing ();
    }

    UnloadTexture (texture);
    exit ();
    return 0;
}
