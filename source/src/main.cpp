#include "camera/camera.hpp"
#include "geometry/Object.hpp"
#include "geometry/Plane.hpp"
#include "geometry/Sphere.hpp"
#include "light/emitter.hpp"
#include "material/Diffuse.hpp"
#include "material/Glossy.hpp"
#include "math/angle.hpp"
#include "math/consts.hpp"
#include "math/color/sRGB/sRGB.hpp"
#include "math/random/random.hpp"
#include "math/vec/vec.hpp"
#include "ray/ray.hpp"
#include "raylib/raylib.h"
#include "sodium.h"
#include "funcs.hpp"
#include <iostream>

int main ()
{
    if (int I = sodium_init (); I != 0)
    {
        std::cout << "ERROR: Failed to initialize RNG." << std::endl;
        return 1;
    }

    camera3D camera = {vec3 {0.75f, -0.4f, 0.5f}, vec3 {-2.0f, 0.5f, 0.0f}, 90.0f, 1920, 1080};
    int pixel_count = camera.width * camera.height;

    std::vector <std::unique_ptr <Object>> scene;
    scene.emplace_back (std::make_unique <Plane> (0.0f, -2.5f, 2.5f, -2.5f, 2.5f, vec3 {0.0f, 0.0f, 1.0f}, std::make_shared <Diffuse> (vec3{0.85f, 0.85f, 0.85f})));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.5f, 0.5f, 0.2501f}, 0.25f, std::make_shared <Glossy> (vec3 {0.96f, 0.96f, 0.96f}, 0.01f)));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.5f, -0.5f, 0.2501f}, 0.25f, std::make_shared <Diffuse> (vec3 {0.0f, 0.85f, 0.3f})));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.1f, 0.3f, 0.1251f}, 0.125f, std::make_shared <Diffuse> (vec3 {0.2f, 0.0f, 0.85f})));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.12f, -0.15f, 0.4f}, 0.125f, std::make_shared <Diffuse> (vec3 {0.3f, 0.5f, 0.85f})));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.02f, -0.1f, 0.0501f}, 0.05f, std::make_shared <Diffuse> (vec3 {0.1f, 0.1f, 0.85f})));
     
    Sphere lamp = {vec3 {1.0f, 1.0f, 2.0f}, 1.0f, Emitter {vec3 {1.0f, 1.0f, 1.0f}, 5.0f}};
    scene.emplace_back (std::make_unique <Sphere> (lamp));

    int max_bounces = 6;
    int max_samples = 8;

    std::vector <Color> pixels (static_cast <std::vector <Color>::size_type> (pixel_count), BLACK);
    Image img = GenImageColor (camera.width, camera.height, BLACK);
    init (camera);
    Texture2D texture = LoadTextureFromImage (img);
    UnloadImage (img);
    std::vector <vec3> accum_buffer (static_cast <std::vector <Color>::size_type> (pixel_count), vec3 {});

    for (int samples_done = 0; samples_done < max_samples; samples_done++)
    {
        for (int y = 0; y < camera.height; y++)
        {
            for (int x = 0; x < camera.width; x++)
            {
                ray r = cast_ray_from_camera (camera, static_cast <float> (x), static_cast <float> (y));
                vec3 color = trace_ray (scene, r, max_bounces);
                accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera.width) + x)].add_rw (color);
            }
        }

        std::cout << std::endl << "SPP: " << samples_done;
    }

    std::cout << std::endl << "SPP: " << max_samples << std::endl;

    for (int y = 0; y < camera.height; y++)
    {
        for (int x = 0; x < camera.width; x++)
        {
            accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera.width) + x)].div_rw (static_cast <float> (max_samples));
            clamp_color (accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera.width) + x)], 0.0f, 1.0f);
            convert_linear_RGB_to_sRGB (accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera.width) + x)]);

            pixels [static_cast <std::vector <Color>::size_type> ((y * camera.width) + x)] = Color
            {
                static_cast <unsigned char> (accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera.width) + x)].x * 255),
                static_cast <unsigned char> (accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera.width) + x)].y * 255),
                static_cast <unsigned char> (accum_buffer [static_cast <std::vector <vec3>::size_type> ((y * camera.width) + x)].z * 255),
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
