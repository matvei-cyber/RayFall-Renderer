#include "kernel.hpp"

int main ()
{
    camera3D camera = {vec3 {0.75f, -0.4f, 0.5f}, vec3 {-2.0f, 0.5f, 0.0f}, 90.0f, 1920.0f, 1080.0f};

    std::vector <std::unique_ptr <Object>> scene;

    scene.emplace_back (std::make_unique <Plane> (0.0f, -2.5f, 2.5f, -2.5f, 2.5f, vec3 {0.0f, 0.0f, 1.0f}, std::make_shared <Diffuse> (vec3 {0.85f, 0.85f, 0.85f})));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.5f, 0.5f, 0.2501f}, 0.25f, std::make_shared <Glossy> (vec3 {0.96f, 0.96f, 0.96f}, 0.01f)));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.5f, -0.5f, 0.2501f}, 0.25f, std::make_shared <Diffuse> (vec3 {0.0f, 0.85f, 0.3f})));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.1f, 0.3f, 0.1251f}, 0.125f, std::make_shared <Diffuse> (vec3 {0.2f, 0.0f, 0.85f})));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.12f, -0.15f, 0.4f}, 0.125f, std::make_shared <Diffuse> (vec3 {0.3f, 0.5f, 0.85f})));
    scene.emplace_back (std::make_unique <Sphere> (vec3 {-0.02f, -0.1f, 0.0501f}, 0.05f, std::make_shared <Diffuse> (vec3 {0.1f, 0.1f, 0.85f})));
    Sphere lamp = {vec3 {1.0f, 1.0f, 2.0f}, 1.0f, Emitter {vec3 {1.0f, 1.0f, 1.0f}, 5.0f}};
    scene.emplace_back (std::make_unique <Sphere> (lamp));

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
                    ray ray = cast_ray_from_camera (camera, (float) j, (float) i);
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
                    clamp_color (final_color, 0.0f, 1.0f);
                    convert_linear_RGB_to_sRGB (final_color);

                    pixels [(size_t) i] = Color
                    {
                        (unsigned char) (final_color.x * 255),
                        (unsigned char) (final_color.y * 255),
                        (unsigned char) (final_color.z * 255), 255
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
