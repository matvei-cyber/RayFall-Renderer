#include "geometry/Plane.hpp"
#include "light/emitter.hpp"
#include "material/Diffuse.hpp"
#include "material/Glossy.hpp"
#include "math/color/sRGB/sRGB.hpp"
#include "math/random/random.hpp"
#include "raylib/raylib.h"
#include "funcs.hpp"
#include "xml/parser.hpp"
#include <iostream>

int main ()
{
    std::vector <Object*> scene = parse_scene_from_xml ();
    camera3D camera = parse_camera_from_xml ();

    vec2 camera_size = camera.get_size ();
    int pixel_count = camera_size.x * camera_size.y;

    std::mt19937 gen (0);
    std::uniform_real_distribution <float> dist (-1.0f, 1.0f);

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
                main_path_tracing (camera, x, y, scene, max_bounces, gen, dist, accum_buffer);
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
