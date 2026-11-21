#include "kernel.hpp"

int main ()
{
    camera3D camera = {vec3 {0.75f, -0.4f, 0.5f}, vec3 {-2.0f, 0.5f, 0.0f}, 90.0f, 1920.0f, 1080.0f};

    std::vector <std::unique_ptr <Object>> scene;

    scene.emplace_back (std::make_unique <Plane> (0.0f, -2.5f, 2.5f, -2.5f, 2.5f, vec3 {0.0f, 0.0f, 1.0f}, vec3 {0.85f, 0.85f, 0.85f}));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.5f, 0.5f, 0.2501f}, 0.25f, vec3 {0.0f, 1.0f, 0.25f}));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.5f, -0.5f, 0.2501f}, 0.25f, vec3 {0.8f, 0.0f, 0.0f}));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.1f, 0.3f, 0.1251f}, 0.125f, vec3 {0.0f, 0.25f, 0.8f}));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.12f, -0.15f, 0.4f}, 0.125f, vec3 {0.5f, 0.0f, 0.8f}));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.02f, -0.1f, 0.0501f}, 0.05f, vec3 {0.0f, 0.8f, 0.0f}));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {1.0f, 1.0f, 2.0f}, 0.5f, Emitter {vec3 {1.0f, 1.0f, 1.0f}, 25.0f}));

    std::vector <Color> pixels ((long long unsigned int) (camera.width * camera.height), BLACK);
    Image img = GenImageColor ((int) camera.width, (int) camera.height, BLACK);
    init (camera);
    Texture2D texture = LoadTextureFromImage (img);
    UnloadImage (img);

    std::vector <vec3> accum_buffer ((size_t) (camera.width * camera.height), vec3 {0.0f, 0.0f, 0.0f});
    vec3 pixel_color;
    int samples_count = 128;
    int samples_done = 0;
    int max_bounces = 6;
    bool pause = false;

    while (!WindowShouldClose ())
    {
        if (IsKeyPressed (KEY_P))
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
                    pixel_color = trace_ray (scene, ray, max_bounces);
                    accum_buffer [(long long unsigned int) ((float) i * camera.width + (float) j)].add_rw (pixel_color);
                }
            }

            samples_done ++;

            if (((samples_done % 10) == 0) || (samples_done == samples_count) || (samples_done == 1))
            {
                for (int i = 0; i < (int) accum_buffer.size (); i++)
                {
                    vec3 final_color = accum_buffer [(size_t) i].div ((float) samples_done);
                    convert_linear_RGB_to_sRGB (final_color);

                    pixels [(size_t) i] = Color
                    {
                        (unsigned char) (clamp (final_color.x, 0.0f, 1.0f) * 255),
                        (unsigned char) (clamp (final_color.y, 0.0f, 1.0f) * 255),
                        (unsigned char) (clamp (final_color.z, 0.0f, 1.0f) * 255), 255
                    };
                }

                UpdateTexture (texture, pixels.data ());
            }
        }

        BeginDrawing ();
            ClearBackground (WHITE);
            DrawTexture (texture, 0, 0, WHITE);
            DrawText (TextFormat ("Samples: %d%s", samples_done, pause ? " [PAUSED]" : ""), 25, 25, 24, Color {255, 255, 255, 255});
        EndDrawing ();
    }

    UnloadTexture (texture);
    exit ();
    return 0;
}
