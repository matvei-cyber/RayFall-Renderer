#include "kernel.hpp"
#include <thread>
#include <atomic>
#include <chrono>
#include <algorithm>

struct render_worker
{
    std::vector <vec3> accum_buffer;
    std::atomic <int> samples_done {0};
    std::thread thread;

    render_worker (const int width, const int height) : accum_buffer ((long long unsigned int) (width * height)) {}
};

void worker_loop (const std::vector <std::unique_ptr <Object>>& scene, const int max_bounces, const camera3D& camera, render_worker& worker, std::atomic <bool>& running)
{
    while (running.load (std::memory_order_relaxed))
    {
        for (int y = 0; y < camera.height; y++)
        {
            if (!running.load (std::memory_order_relaxed)) return;

            for (int x = 0; x < camera.width; x++)
            {
                ray current_ray = cast_ray_from_camera (camera, (float) x, (float) y);
                vec3 color = trace_ray (scene, current_ray, max_bounces);
                worker.accum_buffer [(long long unsigned int) ((y * camera.width) + x)].add_rw (color);
            }
        }
        worker.samples_done.fetch_add (1, std::memory_order_release);
    }
}

int main ()
{
    camera3D camera = {vec3 {0.75f, -0.4f, 0.5f}, vec3 {-2.0f, 0.5f, 0.0f}, 90.0f, 1920, 1080};

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

    unsigned int num_threads = std::max (1u, std::thread::hardware_concurrency ());
    std::atomic <bool> running {true};
    std::vector <std::unique_ptr <render_worker>> workers;
    workers.reserve (num_threads);

    for (unsigned int i = 0; i < num_threads; i++)
    {
        workers.push_back (std::make_unique <render_worker> (camera.width, camera.height));
    }
    for (unsigned int i = 0; i < num_threads; i++)
    {
        workers [i] -> thread = std::thread (worker_loop, std::cref (scene), 6, std::cref (camera), std::ref (*workers [i]), std::ref (running));
    }

    auto last_update = std::chrono::steady_clock::now ();
    int total_samples_rendered = 0;
    bool pause = false;

    while (!WindowShouldClose ())
    {
        auto now = std::chrono::steady_clock::now ();
        double elapsed = std::chrono::duration <double> (now - last_update).count ();

        if (IsKeyPressed (KEY_P))
        {
            pause = !pause;
        }

        if ((elapsed >= 2.0) && !pause)
        {
            last_update = now;

            std::vector<int> current_samples(num_threads);
            long long sum_samples = 0;

            for (unsigned int i = 0; i < num_threads; i++)
            {
                current_samples [i] = workers [i] -> samples_done.load (std::memory_order_acquire);
                sum_samples += current_samples [i];
            }
            total_samples_rendered = (int) sum_samples;

            if (sum_samples > 0)
            {
                float inv_samples = 1.0f / static_cast <float> (sum_samples);

                for (int p = 0; p < (camera.width * camera.height); p++)
                {
                    vec3 total_color = {};
                    for (unsigned int i = 0; i < num_threads; i++)
                    {
                        total_color.add_rw (workers [i] -> accum_buffer [(long long unsigned int) p]);
                    }

                    vec3 avg = total_color.mul (inv_samples);
                    clamp_color (avg, 0.0f, 1.0f);
                    convert_linear_RGB_to_sRGB (avg);

                    pixels [(long long unsigned int) p] =
                    {
                        Color
                        {
                            static_cast <unsigned char> (avg.x * 255),
                            static_cast <unsigned char> (avg.y * 255),
                            static_cast <unsigned char> (avg.z * 255),
                            255
                        }
                    };
                }

                UpdateTexture (texture, pixels.data ());
            }
        }

        BeginDrawing ();

            ClearBackground (BLACK);
            DrawTexture (texture, 0, 0, WHITE);

            if (pause)
            {
                DrawText (TextFormat ("Threads: %d | Total SPP: %d [PAUSED]", num_threads, total_samples_rendered), 10, 10, 20, WHITE);

            } else
            {
                DrawText (TextFormat ("Threads: %d | Total SPP: %d", num_threads, total_samples_rendered), 10, 10, 20, WHITE);
            }

        EndDrawing ();
    }

    running.store (false);
    for (auto& w : workers)
    {
        if (w -> thread.joinable ())
        {
            w -> thread.join ();
        }
    }

    UnloadTexture (texture);
    exit ();
    return 0;
}
