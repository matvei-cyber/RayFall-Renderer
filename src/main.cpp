#include "kernel.hpp"
#include "raylib.h"

int main ()
{
    camera3D camera = {vec3 {1.0f, -0.5f, 0.5f}, vec3 {-0.15f, 0.15f, 0.0f}, 75.0f, 1920.0f, 1080.0f};

    std::vector <std::unique_ptr <Object>> scene;
    scene.emplace_back (std::make_unique <Plane> (0.0f, -0.5f, 0.5f, -0.5f, 0.5f, vec3 {0.85f, 0.85f, 0.85f}));
    scene.emplace_back (std::make_unique <sphere> (vec3 {0.15f, 0.15f, 0.125f}, 0.125f, vec3 {1.0f, 0.0f, 0.1f}));
    scene.emplace_back (std::make_unique <sphere> (vec3 {-0.15f, -0.15f, 0.125f}, 0.125f, vec3 {0.8f, 0.0f, 1.0f}));

    std::vector <Color> pixels ((long long unsigned int) (camera.width * camera.height), BLACK);
    Image img = GenImageColor ((int) camera.width, (int) camera.height, BLACK);
    init (camera);
    Texture2D texture = LoadTextureFromImage (img);
    UnloadImage (img);

    std::vector <vec3> accum_buffer ((size_t) (camera.width * camera.height), vec3 {0.0f, 0.0f, 0.0f});
    vec3 pixel_color;
    int samples_count = 128;
    int samples_done = 0;
    bool pause = false;

    while (!WindowShouldClose ())
    {
        if (IsKeyPressed (KEY_S))
        {
            pause = !pause;
        }

        if ((samples_done < samples_count) && !pause)
        {
            for (int i = 0; i < (int) camera.height; i++)
            {
                for (int j = 0; j < (int) camera.width; j++)
                {
                    ray ray = cast_ray (camera, (float) j, (float) i);
                    pixel_color = trace_ray (scene, ray, 12);
                    accum_buffer [(long long unsigned int) ((float) i * camera.width + (float) j)].add_rw (pixel_color);
                }
            }

            samples_done ++;

            for (int i = 0; i < (int) accum_buffer.size (); i++)
            {
                vec3 final_color = accum_buffer [(long long unsigned int) i];
                final_color.div_rw ((float) samples_done);
                convert_linear_RGB_to_sRGB (final_color);

                pixels [(long long unsigned int) i] = Color
                {
                    (unsigned char) (clamp (final_color.x, 0.0f, 1.0f) * 255),
                    (unsigned char) (clamp (final_color.y, 0.0f, 1.0f) * 255),
                    (unsigned char) (clamp (final_color.z, 0.0f, 1.0f) * 255), 255
                };
            }

            UpdateTexture (texture, pixels.data ());
        }

        BeginDrawing ();

            ClearBackground (WHITE);
            DrawTexture (texture, 0, 0, WHITE);
            DrawText (TextFormat ("Samples: %d", samples_done), 25, 25, 24, Color {0, 255, 100, 255});

        EndDrawing ();
    }

    UnloadTexture (texture);
    exit ();
    return 0;
}
