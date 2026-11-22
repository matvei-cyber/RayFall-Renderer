#include "kernel.hpp"
#include <thread>
#include <chrono>
#include <vector>

volatile bool is_running = true;
volatile bool is_paused = false;
volatile int total_samples = 0;

void render_worker (
    const std::vector <std::unique_ptr <Object>>& scene,
    const camera3D& camera,
    int max_bounces,
    int max_samples,
    std::vector<vec3>& my_buffer)
{
    while (is_running)
    {
        if (is_paused || total_samples >= max_samples)
        {
            std::this_thread::sleep_for (std::chrono::milliseconds (50));
            continue;
        }

        for (int y = 0; y < camera.height; y++)
        {
            if (!is_running) return;

            for (int x = 0; x < camera.width; x++)
            {
                ray r = cast_ray_from_camera (camera, (float) x, (float) y);
                vec3 color = trace_ray (scene, r, max_bounces);
                my_buffer [y * camera.width + x].add_rw (color);
            }
        }

        total_samples += 1;
    }
}

int main ()
{
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
    int max_samples = 128;

    std::vector <Color> pixels (pixel_count, BLACK);
    Image img = GenImageColor (camera.width, camera.height, BLACK);
    init (camera);
    Texture2D texture = LoadTextureFromImage (img);
    UnloadImage (img);

    int num_threads = std::max (1u, std::thread::hardware_concurrency ());
    std::vector <std::vector <vec3>> buffers (num_threads, std::vector <vec3> (pixel_count));
    std::vector <std::thread> threads;

    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back([&, i] ()
        {
            render_worker (scene, camera, max_bounces, max_samples, buffers [i]);
        });
    }

    auto last_update = std::chrono::steady_clock::now ();
    auto last_frame_time = last_update;
    float render_time = 0.0f;

    while (!WindowShouldClose ())
    {
        if (IsKeyPressed (KEY_P))
        {
            is_paused = !is_paused;
        }

        auto now = std::chrono::steady_clock::now ();
        float dt = std::chrono::duration <float> (now - last_frame_time).count ();
        last_frame_time = now;
        double elapsed = std::chrono::duration <double> (now - last_update).count ();
        int samples_now = total_samples;

        if (!is_paused && samples_now < max_samples)
        {
            render_time += dt;
        }

        if ((elapsed >= 2.0) && samples_now > 0)
        {
            last_update = now;
            float inv_samples = 1.0f / (float) samples_now;

            for (int p = 0; p < pixel_count; p++)
            {
                vec3 total_color = {0, 0, 0};
                for (int t = 0; t < num_threads; t++)
                {
                    total_color.add_rw (buffers [t][p]);
                }

                vec3 avg = total_color.mul (inv_samples);
                clamp_color (avg, 0.0f, 1.0f);
                convert_linear_RGB_to_sRGB (avg);

                pixels [p] = Color
                {
                    (unsigned char) (avg.x * 255),
                    (unsigned char) (avg.y * 255),
                    (unsigned char) (avg.z * 255),
                    255
                };
            }

            UpdateTexture (texture, pixels.data ());
        }

        BeginDrawing ();
            ClearBackground (BLACK);
            DrawTexture (texture, 0, 0, WHITE);

            if (is_paused)
            {
                DrawText (TextFormat ("Threads: %d | Total SPP: %d / %d | Time: %.2fs [PAUSED]", num_threads, samples_now, max_samples, render_time), 10, 10, 20, WHITE);
            }
            else if (samples_now >= max_samples)
            {
                DrawText (TextFormat ("Threads: %d | Total SPP: %d / %d | Time: %.2fs [DONE]", num_threads, samples_now, max_samples, render_time), 10, 10, 20, GREEN);
            }
            else
            {
                DrawText (TextFormat ("Threads: %d | Total SPP: %d / %d | Time: %.2fs", num_threads, samples_now, max_samples, render_time), 10, 10, 20, WHITE);
            }

        EndDrawing ();
    }

    is_running = false;

    for (int i = 0; i < num_threads; i++)
    {
        if (threads [i].joinable ())
        {
            threads [i].join ();
        }
    }

    UnloadTexture (texture);
    exit ();
    return 0;
}
